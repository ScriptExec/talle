#pragma once
#include <array>
#include <concepts>

#include <talle/render/frame_side.hpp>
#include <talle/utils/utf8_data.hpp>

namespace talle::meta
{
	template<typename frame_style_type>
	concept frame_style = requires(frame_style_type style, frame_side sides)
	{
		{ style.get(sides) } -> std::same_as<utf8_data>;
	};
}
