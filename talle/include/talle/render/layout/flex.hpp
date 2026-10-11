#pragma once
#include <cstdint>

namespace talle
{
	enum class flex : uint8_t
	{
		// Aligns items to the start of the container
		start,
		// Aligns items to the end of the container
		end,
		// Aligns items to the center of the container
		center,
		// Adds excess space between items
		space_between,
		// Adds excess space around items
		space_around,
		// Adds excess space evenly before, between, and after items
		space_evenly,
	};
}
