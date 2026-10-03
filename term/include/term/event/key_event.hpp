#pragma once
#include <term/input/key.hpp>
#include <term/input/key_modifier.hpp>

namespace term
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
