#pragma once
#include <array>
#include <cstdint>
#include <talle/render/frame_side.hpp>
#include <talle/utils/utf8_data.hpp>
#include <talle/render/frame/impl/frame.hpp>

namespace talle::frame
{
	struct heavy_line : public impl::frame
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

		heavy_line(size_t hash) : impl::frame{ hash } {}

		virtual utf8_data get(frame_side sides) const override
		{
			return glyphs[static_cast<uint8_t>(sides) & 0x0F];
		}

		virtual std::unique_ptr<frame> clone() const override
		{
			return std::make_unique<heavy_line>(hash());
		}
	};
}
