#pragma once
#include <cstdint>

namespace talle
{
	struct position
	{
		constexpr position() = default;
		constexpr position(uint16_t x, uint16_t y) : x{ x }, y{ y } {}

		uint16_t x{};
		uint16_t y{};

		constexpr bool operator==(const position& other) const
		{
			return x == other.x and y == other.y;
		}

		constexpr bool operator!=(const position& other) const
		{
			return !(*this == other);
		}

		constexpr bool operator<(const position& other) const
		{
			return (y < other.y) or (y == other.y and x < other.x);
		}
	};
}
