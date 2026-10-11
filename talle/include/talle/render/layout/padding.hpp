#pragma once
#include <cstdint>

namespace talle
{
	struct padding
	{
		uint16_t top = 0;
		uint16_t right = 0;
		uint16_t bottom = 0;
		uint16_t left = 0;

		constexpr padding() = default;
		constexpr padding(uint16_t uniform) : top{ uniform }, right{ uniform }, bottom{ uniform }, left{ uniform } {}
		constexpr padding(uint16_t vertical, uint16_t horizontal) : top{ vertical }, right{ horizontal }, bottom{ vertical }, left{ horizontal } {}
		constexpr padding(uint16_t top, uint16_t right, uint16_t bottom, uint16_t left) : top{ top }, right{ right }, bottom{ bottom }, left{ left } {}
	};
}
