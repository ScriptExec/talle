#include <talle/render/widgets/layout.hpp>

#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include <numeric>

namespace talle
{
	namespace detail
	{
		static uint16_t to_size(int value)
		{
			return static_cast<uint16_t>(std::clamp<int>(value, std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max()));
		}

		static int clamp_dimension(int value, const std::optional<uint16_t>& minimum, const std::optional<uint16_t>& maximum)
		{
			if (minimum) value = std::max(value, static_cast<int>(*minimum));
			if (maximum) value = std::min(value, static_cast<int>(*maximum));
			return std::max<int>(0, value);
		}

		static int resolve_dimension
		(
			int preferred,
			int available,
			const std::optional<uint16_t>& fixed,
			const std::optional<float>& percentage,
			const std::optional<uint16_t>& min_value,
			const std::optional<uint16_t>& max_value
		)
		{
			int result = preferred;

			if (fixed)
			{
				result = *fixed;
			}
			else if (percentage)
			{
				const float percent = std::clamp(*percentage, 0.0f, 1.0f);
				result = static_cast<int>(std::lround(percent * available));
			}
			return clamp_dimension(result, min_value, max_value);
		}

		static int percentage_size(float percentage, int available)
		{
			percentage = std::clamp(percentage, 0.0f, 1.0f);
			return static_cast<int>(std::lround(percentage * available));
		}

		static int total_size(const std::vector<int>& sizes)
		{
			int result{};
			for (auto size : sizes)
			{
				result += size;
			}
			return result;
		};

		static void distribute_space
		(
			std::vector<int>& sizes,
			const std::vector<int>& minimum,
			const std::vector<int>& maximum,
			const std::vector<float>& grow,
			const std::vector<float>& shrink,
			int available_size
		)
		{
			// recalculates weights after each child reaches a limit
			for (size_t iteration = 0; iteration < sizes.size() + 1; ++iteration)
			{
				int free_size = available_size - total_size(sizes);
				if (free_size == 0) return;

				std::vector<float> weights(sizes.size(), 0.0f);
				float weight_sum = 0.0f;

				for (size_t i = 0; i < sizes.size(); ++i)
				{
					if (free_size > 0 and sizes[i] < maximum[i])
					{
						weights[i] = std::max(0.0f, grow[i]);
					}
					else if (free_size < 0 and sizes[i] > minimum[i])
					{
						weights[i] = std::max(0.0f, shrink[i]) * std::max<int>(1, sizes[i]);
					}
					weight_sum += weights[i];
				}

				if (weight_sum <= 0.0f) return;

				int remaining_size = std::abs(free_size);
				bool changed = false;

				std::vector<int> base_shares(sizes.size(), 0);
				int total_distributed = 0;

				for (size_t i = 0; i < sizes.size(); ++i)
				{
					if (weights[i] <= 0.0f) continue;

					int share = static_cast<int>(std::floor(remaining_size * weights[i] / weight_sum));
					base_shares[i] = share;
					total_distributed += share;
				}

				for (size_t i = 0; i < sizes.size(); ++i)
				{
					if (base_shares[i] == 0) continue;

					if (free_size > 0)
					{
						int delta = std::min(base_shares[i], maximum[i] - sizes[i]);
						sizes[i] += delta;
						changed |= delta > 0;
					}
					else
					{
						int delta = std::min(base_shares[i], sizes[i] - minimum[i]);
						sizes[i] -= delta;
						changed |= delta > 0;
					}
				}

				// distributes remainder pixels one at a time
				int remainder = remaining_size - total_distributed;
				for (int r = 0; r < remainder; ++r)
				{
					size_t i = r % sizes.size();

					// finds next child
					for (size_t attempts = 0; attempts < sizes.size(); ++attempts)
					{
						if (weights[i] > 0.0f)
						{
							if (free_size > 0)
							{
								if (sizes[i] < maximum[i])
								{
									sizes[i]++;
									changed = true;
									break;
								}
							}
							else
							{
								if (sizes[i] > minimum[i])
								{
									sizes[i]--;
									changed = true;
									break;
								}
							}
						}
						i = (i + 1) % sizes.size();
					}
				}

				if (!changed) return;
			}
		}
	}

	layout& layout::set_flex(flex value)
	{
		flex_ = value;
		return *this;
	}

	layout& layout::set_alignment(alignment value)
	{
		alignment_ = value;
		return *this;
	}

	layout& layout::set_padding(const padding& value)
	{
		padding_ = value;
		return *this;
	}

	layout& layout::set_direction(direction value)
	{
		direction_ = value;
		return *this;
	}

	layout& layout::set_gap(uint16_t value)
	{
		gap_ = value;
		return *this;
	}

	rect layout::inner_bounds() const
	{
		//TODO: clamp values
		rect result = bounds_;
		result.start.x += padding_.left;
		result.start.y += padding_.top;
		result.end.x -= padding_.right;
		result.end.y -= padding_.bottom;
		if (has_frame())
		{
			result.start.x += 1;
			result.start.y += 1;
			result.end.x -= 1;
			result.end.y -= 1;
		}
		return result;
	}

	bool layout::has_frame() const
	{
		return frame_type_ != nullptr;
	}

	size layout::measure(size available) const
	{
		int main_total = 0;
		int cross_max = 0;
		int child_count = 0;

		const int available_width = available.width;
		const int available_height = available.height;

		auto bounds = inner_bounds();
		const int inner_width = std::max<int>(0, available_width - bounds.width());
		const int inner_height = std::max<int>(0, available_height - bounds.height());

		for (const auto& child : children_)
		{
			if (!child->item->visible()) continue;

			size preferred_size = child->item->measure
			({
				detail::to_size(inner_width),
				detail::to_size(inner_height)
			});

			int width = preferred_size.width;
			int height = preferred_size.height;

			const auto& c = child->constraints;

			if (c.width.has_value())
			{
				width = *c.width;
			}
			else if (c.width_percentage.has_value())
			{
				width = detail::percentage_size(*c.width_percentage, inner_width);
			}

			if (c.height.has_value())
			{
				height = *c.height;
			}
			else if (c.height_percentage.has_value()) {
				height = detail::percentage_size(*c.height_percentage, inner_height);
			}

			width = detail::clamp_dimension(width, c.min_width, c.max_width);
			height = detail::clamp_dimension(height, c.min_height, c.max_height);

			if (direction_ == direction::horizontal)
			{
				main_total += width;
				cross_max = std::max(cross_max, height);
			}
			else
			{
				main_total += height;
				cross_max = std::max(cross_max, width);
			}
			++child_count;
		}

		if (child_count > 1)
		{
			main_total += (child_count - 1) * gap_;
		}

		int width;
		int height;

		if (direction_ == direction::horizontal)
		{
			width = main_total + padding_.left + padding_.right;
			height = cross_max + padding_.top + padding_.bottom;
		}
		else
		{
			width = cross_max + padding_.left + padding_.right;
			height = main_total + padding_.top + padding_.bottom;
		}

		return { detail::to_size(width), detail::to_size(height) };
	}

	void layout::arrange(const rect& bounds)
	{
		struct item_data
		{
			child<widget>* child;
			int main_size;
			int cross_size;
			int min_main;
			int max_main;
			int min_cross;
			int max_cross;
			float grow;
			float shrink;
		};

		bounds_ = bounds;

		if (!visible()) return;

		// inner content rectangle
		const int inner_x = static_cast<int>(bounds.start.x) + padding_.left;
		const int inner_y = static_cast<int>(bounds.start.y) + padding_.top;

		const int inner_width = std::max<int>(0, static_cast<int>(bounds.width()) - padding_.left - padding_.right);
		const int inner_height = std::max<int>(0, static_cast<int>(bounds.height()) - padding_.top - padding_.bottom);

		const int available_main = direction_ == direction::horizontal ? inner_width : inner_height;
		const int available_cross = direction_ == direction::horizontal ? inner_height : inner_width;

		// collects visible children and calculates their base sizes
		std::vector<item_data> items;

		for (auto& child : children_)
		{
			if (!child->item->visible()) continue;

			size preferred = child->item->measure
			({
				detail::to_size(inner_width),
				detail::to_size(inner_height)
			});

			const auto& c = child->constraints;

			int width = detail::resolve_dimension
			(
				preferred.width,
				inner_width,
				c.width,
				c.width_percentage,
				c.min_width,
				c.max_width
			);

			int height = detail::resolve_dimension
			(
				preferred.height,
				inner_height,
				c.height,
				c.height_percentage,
				c.min_height,
				c.max_height
			);

			static constexpr uint16_t min_dimension = std::numeric_limits<uint16_t>::min();
			static constexpr uint16_t max_dimension = std::numeric_limits<uint16_t>::max();

			int main_size{};
			int cross_size{};
			int min_main{};
			int max_main{};
			int min_cross{};
			int max_cross{};
			switch (direction_)
			{
				case direction::horizontal:
				{
					main_size = width;
					cross_size = height;
					min_main = c.min_width.value_or(min_dimension);
					max_main = c.max_width.value_or(max_dimension);
					min_cross = c.min_height.value_or(min_dimension);
					max_cross = c.max_height.value_or(max_dimension);
				}
				break;
				case direction::vertical:
				{
					main_size = height;
					cross_size = width;
					min_main = c.min_height.value_or(min_dimension);
					max_main = c.max_height.value_or(max_dimension);
					min_cross = c.min_width.value_or(min_dimension);
					max_cross = c.max_width.value_or(max_dimension);
				}
				break;
			}
			

			float grow = std::max(0.0f, child->layout.grow_factor);
			float shrink = std::max(0.0f, child->layout.shrink_factor);

			// fill requests growth when no explicit grow weight has been provided
			if (c.fill() and grow == 0.0f)
			{
				grow = 1.0f;
			}

			items.push_back
			({
				child.get(),
				std::clamp(main_size, min_main, max_main),
				std::clamp(cross_size, min_cross, max_cross),
				min_main,
				max_main,
				min_cross,
				max_cross,
				grow,
				shrink
			});
		}

		if (items.empty()) return;

		// reserves the gaps before flex distribution.
		const int gap_total = static_cast<int>(items.size() - 1) * gap_;
		const int space_for_children = std::max<int>(0, available_main - gap_total);

		std::vector<int> sizes;
		std::vector<int> minimums;
		std::vector<int> maximums;
		std::vector<float> grows;
		std::vector<float> shrinks;

		for (const auto& item : items)
		{
			sizes.push_back(item.main_size);
			minimums.push_back(item.min_main);
			maximums.push_back(item.max_main);
			grows.push_back(item.grow);
			shrinks.push_back(item.shrink);
		}

		if (flex_ == flex::space_evenly)
		{
			int equal_size = space_for_children / static_cast<int>(items.size());
			for (auto& size : sizes)
			{
				size = equal_size;
			}
		}
		else
		{
			// remaining or insufficient space
			detail::distribute_space
			(
				sizes,
				minimums,
				maximums,
				grows,
				shrinks,
				space_for_children
			);
		}

		// calculates the leftover space for main direction alignment
		const int used_space = detail::total_size(sizes);
		const int free_space = std::max<int>(0, available_main - gap_total - used_space);

		int leading_space = 0;
		int extra_space_between = 0;

		switch (flex_)
		{
			case flex::start: break;
			case flex::end:
			{
				leading_space = free_space;
			}
			break;
			case flex::center:
			{
				leading_space = free_space / 2;
			}
			break;
			case flex::space_between:
			{
				if (items.size() > 1)
				{
					extra_space_between = free_space / static_cast<int>(items.size() - 1);
				}
			}
			break;
			case flex::space_evenly:
			{
				leading_space = free_space / static_cast<int>(items.size() + 1);
				extra_space_between = leading_space;
			}
			break;
			case flex::space_around:
			{
				extra_space_between = free_space / static_cast<int>(items.size());
				leading_space = extra_space_between / 2;
			}
			break;
		}

		// positions and arranges each child
		int cursor = (direction_ == direction::horizontal ? inner_x : inner_y) + leading_space;

		for (size_t i = 0; i < items.size(); ++i)
		{
			auto& item = items[i];

			int main_size = sizes[i];
			int cross_size = item.cross_size;
			int cross_offset = 0;

			if (item.child->constraints.fill() or alignment_ == alignment::stretch)
			{
				cross_size = std::clamp(available_cross, item.min_cross, item.max_cross);
			}
			else
			{
				cross_size = std::clamp(cross_size, item.min_cross, item.max_cross);

				if (alignment_ == alignment::end)
				{
					cross_offset = available_cross - cross_size;
				}
				else if (alignment_ == alignment::center)
				{
					cross_offset = (available_cross - cross_size) / 2;
				}
			}

			cross_offset = std::max<int>(0, cross_offset);

			rect child_bounds;

			if (direction_ == direction::horizontal)
			{
				child_bounds =
				{
					detail::to_size(cursor),
					detail::to_size(inner_y + cross_offset),
					detail::to_size(cursor + main_size),
					detail::to_size(inner_y + cross_offset + cross_size)
				};
			}
			else
			{
				child_bounds =
				{
					detail::to_size(inner_x + cross_offset),
					detail::to_size(cursor),
					detail::to_size(inner_x + cross_offset + cross_size),
					detail::to_size(cursor + main_size)
				};
			}

			item.child->item->arrange(child_bounds);

			cursor += main_size + gap_;

			if (i < items.size() - 1)
			{
				cursor += extra_space_between;
			}
		}
	}

	void layout::render(render::context& ctx) const
	{
		if (!visible()) return;

		if (has_frame())
		{
			ctx.add_frame(*frame_type_, bounds_);
		}

		for (const auto& child : children_)
		{
			if (child->item->visible())
			{
				child->item->render(ctx);
			}
		}
	}
}
