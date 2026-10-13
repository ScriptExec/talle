#include <talle/render/draw_buffer.hpp>

namespace talle
{
	draw_buffer_diff draw_buffer::diff(const draw_buffer& other) const
	{
		draw_buffer_diff diff;
		size min_size{ std::min(size_.width, other.size_.width), std::min(size_.height, other.size_.height) };
		if (min_size.width == 0 or min_size.height == 0) return diff;

		std::optional<draw_buffer_diff_range> current_range;
		auto push_or_merge = [&](position pos)
		{
			if (current_range.has_value())
			{
				if (pos.x == current_range->end.x + 1 and pos.y == current_range->end.y)
				{
					current_range->end = pos;
				}
				else
				{
					diff.ranges.push_back(*current_range);
					current_range = draw_buffer_diff_range{ pos, pos };
				}
			}
			else
			{
				current_range = draw_buffer_diff_range{ pos, pos };
			}
		};

		for (uint16_t y = 0; y < min_size.height; ++y)
		{
			for (uint16_t x = 0; x < min_size.width; ++x)
			{
				position pos{ x, y };
				if (at(pos) != other.at(pos))
				{
					push_or_merge(pos);
				}
			}
		}

		if (current_range.has_value())
		{
			diff.ranges.push_back(*current_range);
			current_range.reset();
		}
		return diff;
	}
}
