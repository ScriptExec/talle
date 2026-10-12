#pragma once
#include <array>
#include <cstdint>
#include <talle/render/frame_side.hpp>
#include <talle/utils/utf8_data.hpp>
#include <talle/render/frame/impl/frame.hpp>

namespace talle::frame
{
	struct rounded_line : public impl::frame
	{
		static constexpr std::array<utf8_data, 16> glyphs
		{
			u8" ", // none
			u8"\u2575", // top
			u8"\u2576", // right
			u8"\u2570", // top | right
			u8"\u2577", // bottom
			u8"\u2502", // top | bottom
			u8"\u256D", // right | bottom
			u8"\u251C", // top | right | bottom
			u8"\u2574", // left
			u8"\u256F", // top | left
			u8"\u2500", // right | left
			u8"\u2534", // top | right | left
			u8"\u256E", // bottom | left
			u8"\u2524", // top | bottom | left
			u8"\u252C", // right | bottom | left
			u8"\u253C", // all
		};

		rounded_line(size_t hash) : impl::frame{ hash } {}

		virtual utf8_data get(frame_side sides) const override
		{
			return glyphs[static_cast<uint8_t>(sides) & 0x0F];
		}

		virtual std::unique_ptr<frame> clone() const override
		{
			return std::make_unique<rounded_line>(hash());
		}
	};
}
