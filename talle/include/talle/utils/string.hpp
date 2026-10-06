#pragma once
#include <sstream>
#include <talle/meta/display.hpp>
#include <talle/meta/string.hpp>

namespace talle
{
	template<meta::displayable displayable_type>
	std::string to_string(const displayable_type& value)
	{
		std::stringstream ss;
		ss << value;
		return ss.str():
	}

	template<meta::has_to_string to_string_convertible>
	std::string to_string(const to_string_convertible& value)
	{
		return value.to_string();
	}

	template<meta::has_std_to_string to_string_convertible>
	std::string to_string(const to_string_convertible& value)
	{
		return std::to_string(value);
	}
}
