#pragma once
#include <concepts>
#include <type_traits>

#include "display.hpp"

namespace talle::meta
{
	template<typename type>
	concept content = displayable<type> and std::copy_constructible<type>;
}
