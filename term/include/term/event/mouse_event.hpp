#pragma once
#include <variant>
#include <term/utils/position.hpp>

namespace term
{
	enum class mouse_button : uint8_t
	{
		left,
		middle,
		right,
	};

	enum class mouse_button_state : uint8_t
	{
		down,
		up,
	};

	enum class mouse_wheel_direction : uint8_t
	{
		up,
		down,
		left,
		right,
	};

	struct mouse_button_event
	{
		mouse_button_event() = default;
		mouse_button_event(mouse_button button, mouse_button_state state) : button{ button }, state{ state } {}
		mouse_button button;
		mouse_button_state state;
	};

	struct mouse_wheel_event
	{
		mouse_wheel_event() = default;
		mouse_wheel_event(mouse_wheel_direction direction, int delta) : direction{ direction }, delta{ delta } {}

		int delta{};
		mouse_wheel_direction direction{};
	};

	struct mouse_move_event
	{
		mouse_move_event() = default;
		mouse_move_event(position pos) : pos{ pos } {}

		position pos;
	};

	struct mouse_event
	{
		std::variant<mouse_button_event, mouse_wheel_event, mouse_move_event> event;
		position pos;

		template<typename type>
		bool is() const
		{
			return std::holds_alternative<type>(event);
		}

		template<typename type>
		type& get()
		{
			return std::get<type>(event);
		}

		template<typename type>
		const type& get() const
		{
			return std::get<type>(event);
		}

		template<typename type>
		type* try_get()
		{
			return std::get_if<type>(&event);
		}

		template<typename type>
		const type* try_get() const
		{
			return std::get_if<type>(&event);
		}
	};
}
