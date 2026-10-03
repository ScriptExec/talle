#pragma once
#include <variant>
#include "mouse_event.hpp"
#include "resize_event.hpp"
#include "focus_changed_event.hpp"

namespace term
{
	struct event
	{
		std::variant<mouse_event, focus_changed_event, resize_event> event;

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
