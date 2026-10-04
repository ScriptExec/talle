#pragma once
#include <cstdint>

namespace talle
{
	enum class key_modifier : uint8_t
	{
		none = 0,
		shift = 1 << 0,
		ctrl = 1 << 1,
		alt = 1 << 2,
		super = 1 << 3,
	};

	struct key_modifiers
	{
		key_modifiers() = default;
		key_modifiers(key_modifier modifier) : modifiers{ static_cast<uint8_t>(modifier) } {}
		key_modifiers(uint8_t modifiers) : modifiers{ modifiers } {}

		uint8_t modifiers{};

		key_modifiers& set(key_modifier modifier, bool value)
		{
			if (value)
			{
				modifiers |= static_cast<uint8_t>(modifier);
			}
			else
			{
				modifiers &= ~static_cast<uint8_t>(modifier);
			}
			return *this;
		}

		bool has(key_modifier modifier) const
		{
			return (modifiers & static_cast<uint8_t>(modifier)) != 0;
		}

		bool is(key_modifier modifier) const
		{
			return modifiers == static_cast<uint8_t>(modifier);
		}

		size_t modifier_count()
		{
			size_t result{};
			for (auto mod :
			{
				key_modifier::shift,
				key_modifier::ctrl,
				key_modifier::alt,
				key_modifier::super,
			})
			{
				if (has(mod))
				{
					++result;
				}
			}
			return result;
		}
	};
}
