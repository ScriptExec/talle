#pragma once
#include <cstdint>
#include "position.hpp"
#include "size.hpp"

namespace talle
{
	struct rect
	{
		rect() = default;
		rect(position start, position end) : start{ start }, end{ end } {}
		rect(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y) : start{ start_x, start_y }, end{ end_x, end_y } {}

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
	};
}
