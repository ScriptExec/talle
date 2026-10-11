#pragma once
#include <cstdint>

namespace talle
{
	enum class frame_side : uint8_t
	{
		none,
		top = 1 << 0,
		right = 1 << 1,
		bottom = 1 << 2,
		left = 1 << 3,
	};

	constexpr frame_side operator|(frame_side left, frame_side right)
	{
		return static_cast<frame_side>(static_cast<uint8_t>(left) | static_cast<uint8_t>(right));
	}

	constexpr frame_side& operator|=(frame_side& left, frame_side right)
	{
		left = left | right;
		return left;
	}
}
