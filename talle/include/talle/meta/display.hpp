#pragma once
#include <concepts>
#include <type_traits>

#include "writer.hpp"

namespace talle::meta
{
	template<typename type>
	concept displayable = writeable<std::ostream, type>;
}
