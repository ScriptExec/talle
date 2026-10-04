#pragma once
#include <variant>
#include "mouse_event.hpp"
#include "resize_event.hpp"
#include "focus_changed_event.hpp"
#include "key_event.hpp"

namespace talle
{
	class event
	{
	public:
		event(const std::variant<key_event, mouse_event, focus_changed_event, resize_event>& data) : data_{ data } {}

	private:
		std::variant<key_event, mouse_event, focus_changed_event, resize_event> data_;

	public:
		template<typename type>
		bool is() const
		{
			return std::holds_alternative<type>(data_);
		}

		template<typename type>
		type& get()
		{
			return std::get<type>(data_);
		}

		template<typename type>
		const type& get() const
		{
			return std::get<type>(data_);
		}

		template<typename type>
		type* try_get()
		{
			return std::get_if<type>(&data_);
		}

		template<typename type>
		const type* try_get() const
		{
			return std::get_if<type>(&data_);
		}
	};
}
