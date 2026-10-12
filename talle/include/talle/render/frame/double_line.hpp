#pragma once
#include <array>
#include <cstdint>
#include <talle/render/frame_side.hpp>
#include <talle/utils/utf8_data.hpp>
#include <talle/render/frame/impl/frame.hpp>

namespace talle::frame
{
	struct double_line : public impl::frame
	{
		static constexpr std::array<utf8_data, 16> glyphs
		{
			u8" ", // none
			u8"\u2551", // top
			u8"\u2550", // right
			u8"\u255A", // top | right
			u8"\u2551", // bottom
			u8"\u2551", // top | bottom
			u8"\u2554", // right | bottom
			u8"\u2560", // top | right | bottom
			u8"\u2550", // left
			u8"\u255D", // top | left
			u8"\u2550", // right | left
			u8"\u2569", // top | right | left
			u8"\u2557", // bottom | left
			u8"\u2563", // top | bottom | left
			u8"\u2566", // right | bottom | left
			u8"\u256C", // all
		};

		double_line(size_t hash) : impl::frame{ hash } {}

		virtual utf8_data get(frame_side sides) const override
		{
			return glyphs[static_cast<uint8_t>(sides) & 0x0F];
		}

		virtual std::unique_ptr<frame> clone() const override
		{
			return std::make_unique<double_line>(hash());
		}
	};
}
