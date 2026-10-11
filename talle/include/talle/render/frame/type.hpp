#pragma once
#include <cstdint>

namespace talle::frame
{
	enum class type : uint8_t
	{
		none,
		single_line,
		double_line,
		heavy_line,
		rounded_line,
	};
}
