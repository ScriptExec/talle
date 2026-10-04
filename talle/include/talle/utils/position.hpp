#pragma once
#include <cstdint>

namespace talle
{
	struct position
	{
		position() = default;
		position(uint16_t x, uint16_t y) : x{ x }, y{ y } {}

		uint16_t x{};
		uint16_t y{};
	};
}
