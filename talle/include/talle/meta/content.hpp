#pragma once
#include <concepts>
#include <type_traits>
#include <string_view>

#include "display.hpp"

namespace talle::meta
{
	template<typename type>
	using content_decay_type = std::conditional_t<
		(std::is_array_v<std::remove_reference_t<type>> or std::is_pointer_v<std::decay_t<type>>)
		and std::is_convertible_v<type, std::string_view>, std::string_view, std::decay_t<type>
	>;

	template<typename type>
	concept content = displayable<content_decay_type<type>> and std::copy_constructible<content_decay_type<type>>;
}
