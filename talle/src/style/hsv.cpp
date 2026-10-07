#include <talle/style/hsv.hpp>
#include <cmath>
#include <algorithm>

namespace talle::style
{
	color::rgb hsv::to_rgb() const
	{
		return hsv_to_rgb(*this);
	}

	color::rgb hsv_to_rgb(hsv color)
	{
		const float c = static_cast<float>(color.v * color.s);
		const float x = static_cast<float>(c * (1.0 - std::abs(std::fmod(color.h / 60.0, 2.0) - 1.0)));
		const float m = static_cast<float>(color.v - c);

		float r = 0;
		float g = 0;
		float b = 0;

		if (color.h < 60)
		{
			r = c;
			g = x;
		}
		else if (color.h < 120)
		{
			r = x;
			g = c;
		}
		else if (color.h < 180)
		{
			g = c;
			b = x;
		}
		else if (color.h < 240)
		{
			g = x;
			b = c;
		}
		else if (color.h < 300)
		{
			r = x;
			b = c;
		}
		else
		{
			r = c;
			b = x;
		}

		return
		{
			static_cast<uint8_t>((r + m) * 255.0),
			static_cast<uint8_t>((g + m) * 255.0),
			static_cast<uint8_t>((b + m) * 255.0)
		};
	}

	hsv rgb_to_hsv(color::rgb color)
	{
		const auto r = static_cast<float>(color.r) / 255.0f;
		const auto g = static_cast<float>(color.g) / 255.0f;
		const auto b = static_cast<float>(color.b) / 255.0f;

		const auto max = std::max({ r, g, b });
		const auto min = std::min({ r, g, b });
		const auto delta = max - min;

		float h = 0.0f;

		if (delta != 0.0f)
		{
			if (max == r)
			{
				h = 60.0f * std::fmod((g - b) / delta, 6.0f);
				if (h < 0.0f)
				{
					h += 360.0f;
				}
			}
			else if (max == g)
			{
				h = 60.0f * ((b - r) / delta + 2.0f);
			}
			else
			{
				h = 60.0f * ((r - g) / delta + 4.0f);
			}
		}
		const auto s = (max == 0.0f) ? 0.0f : delta / max;
		const auto v = max;

		return hsv{ h, s, v };
	}
}
