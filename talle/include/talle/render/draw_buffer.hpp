#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <talle/utils/size.hpp>
#include <talle/utils/rect.hpp>
#include <talle/utils/position.hpp>
#include "cell.hpp"

namespace talle
{
	struct draw_buffer_diff_range
	{
		position start{};
		position end{};
	};

	struct draw_buffer_diff
	{
		std::vector<draw_buffer_diff_range> ranges;

		constexpr bool has_changes() const
		{
			return ranges.size() != 0;
		}
	};

	class draw_buffer
	{
	public:
		draw_buffer() : size_{} {}
		draw_buffer(size size)
		{
			size_ = size;
			resize(size);
		}

	private:
		std::vector<cell> data_;
		size size_;

	public:
		void resize(size size)
		{
			data_.resize(size.width * size.height);
			fill(cell{});
			size_ = size;
		}

		void fill(const cell& value)
		{
			std::fill(data_.begin(), data_.end(), value);
		}

		void fill(const rect& area, const cell& value)
		{
			for (uint16_t y = area.start.y; y <= area.end.y; ++y)
			{
				for (uint16_t x = area.start.x; x <= area.end.x; ++x)
				{
					set({ x, y }, value);
				}
			}
		}

		void set(position pos, const cell& value)
		{
			if (pos.x >= size_.width or pos.y >= size_.height) return;
			uint16_t index = pos.y * size_.width + pos.x;
			data_.at(index) = value;
		}

		size get_size() const
		{
			return size_;
		}

		uint16_t width() const
		{
			return size_.width;
		}

		uint16_t height() const
		{
			return size_.height;
		}

		std::vector<cell>& data()
		{
			return data_;
		}

		const std::vector<cell>& data() const
		{
			return data_;
		}

		cell& at(position pos)
		{
			return data_.at(pos.y * size_.width + pos.x);
		}
		
		const cell& at(position pos) const
		{
			return data_.at(pos.y * size_.width + pos.x);
		}

		cell& operator[](size_t index)
		{
			return data_.at(index);
		}

		const cell& operator[](size_t index) const
		{
			return data_.at(index);
		}

		cell& operator()(position pos)
		{
			return at(pos);
		}

		const cell& operator()(position pos) const
		{
			return at(pos);
		}

		draw_buffer_diff diff(const draw_buffer& other) const;
	};
}
