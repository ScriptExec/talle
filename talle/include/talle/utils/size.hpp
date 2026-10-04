#pragma once
#include <cstdint>

namespace talle
{
	struct size
	{
		size() = default;
		size(uint16_t width, uint16_t height) : width{ width }, height{ height } {}

		uint16_t width{};
		uint16_t height{};
	};
}
