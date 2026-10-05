#pragma once
#include "color.hpp"

namespace talle::style
{
	struct hsv
	{
		/// 0..360
		float h = 0.0;
		/// 0..1
		float s = 1.0;
		/// 0..1
		float v = 1.0;

		color::rgb to_rgb() const;
	};

	color::rgb hsv_to_rgb(hsv color);
	hsv rgb_to_hsv(color::rgb color);

}
