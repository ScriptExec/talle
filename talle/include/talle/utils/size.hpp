#pragma once
#include <cstdint>

namespace talle
{
	struct size
	{
		constexpr size() = default;
		constexpr size(uint16_t width, uint16_t height) : width{ width }, height{ height } {}

		uint16_t width{};
		uint16_t height{};
	};
}
