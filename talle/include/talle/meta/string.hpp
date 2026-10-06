#pragma once
#include <string>
#include <concepts>
#include <type_traits>

#include "display.hpp"

namespace talle::meta
{
	template<typename type>
	concept has_to_string = requires(const type& value)
	{
		{ value.to_string() } -> std::same_as<std::string>;
	};

	template<typename type>
	concept has_std_to_string = requires(const type& value)
	{
		{ std::to_string(value) } -> std::same_as<std::string>;
	};

	template<typename type>
	concept convertible_to_string = displayable<type> or has_to_string<type> or has_std_to_string<type>;
}
