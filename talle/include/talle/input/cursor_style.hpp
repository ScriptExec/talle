#pragma once
#include <cstdint>

namespace talle
{
	enum class cursor_style : uint8_t
	{
		user_default,
		blinking_block,
		steady_block,
		blinking_underline,
		steady_underline,
		blinking_bar,
		steady_bar
	};
}
