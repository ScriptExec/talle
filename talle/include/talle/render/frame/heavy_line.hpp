#pragma once
#include <array>
#include <cstdint>
#include <talle/meta/frame_style.hpp>
#include <talle/render/frame_side.hpp>
#include <talle/utils/utf8_data.hpp>

namespace talle::frame
{
	struct heavy_line
	{
		static constexpr std::array<utf8_data, 16> glyphs
		{
			u8" ", // none
			u8"\u2579", // top
			u8"\u257A", // right
			u8"\u2517", // top | right
			u8"\u257B", // bottom
			u8"\u2503", // top | bottom
			u8"\u250F", // right | bottom
			u8"\u2523", // top | right | bottom
			u8"\u2578", // left
			u8"\u251B", // top | left
			u8"\u2501", // right | left
			u8"\u253B", // top | right | left
			u8"\u2513", // bottom | left
			u8"\u252B", // top | bottom | left
			u8"\u2533", // right | bottom | left
			u8"\u254B", // all
		};

		constexpr utf8_data get(frame_side sides)
		{
			return glyphs[static_cast<uint8_t>(sides) & 0x0F];
		}
	};
}
