#pragma once
#include <talle/input/key.hpp>
#include <talle/input/key_modifier.hpp>

namespace talle
{
	enum class key_event_type
	{
		press,
		repeat,
		release,
	};

	struct key_event
	{
		key key;
		key_event_type type{};
		key_modifiers modifiers;
	};
}
