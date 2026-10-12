#pragma once
#include <array>
#include <cstdint>
#include <talle/render/frame_side.hpp>
#include <talle/utils/utf8_data.hpp>
#include <talle/render/frame/impl/frame.hpp>

namespace talle::frame
{
	struct none : public impl::frame
	{
		virtual utf8_data get([[maybe_unused]] frame_side sides) const override
		{
			return ' ';
		}
	};
}
