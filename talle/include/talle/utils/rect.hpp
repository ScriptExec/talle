#pragma once
#include <cstdint>
#include "position.hpp"
#include "size.hpp"

namespace talle
{
	struct rect
	{
		constexpr rect() = default;
		constexpr rect(position start, position end) : start{ start }, end{ end } {}
		constexpr rect(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y) : start{ start_x, start_y }, end{ end_x, end_y } {}

		position start{};
		position end{};

		constexpr size get_size() const
		{
			return { width(), height() };
		}

		constexpr uint16_t width() const
		{
			return end.x - start.x;
		}

		constexpr uint16_t height() const
		{
			return end.y - start.y;
		}

		constexpr bool contains(position pos) const
		{
			return pos.x >= start.x and pos.x <= end.x and pos.y >= start.y and pos.y <= end.y;
		}
	};
}
