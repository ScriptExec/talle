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

		bool is(key_event_type type) const
		{
			return this->type == type;
		}

		bool is_press() const
		{
			return is(key_event_type::press);
		}

		bool is_repeat() const
		{
			return is(key_event_type::repeat);
		}

		bool is_release() const
		{
			return is(key_event_type::release);
		}
	};
}
