#pragma once
#include <concepts>
#include <talle/render/frame/impl/frame.hpp>

namespace talle::meta
{
	template<typename type>
	concept frame = std::derived_from<type, talle::impl::frame>;
}
