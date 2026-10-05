#pragma once
#include <talle/meta/writer.hpp>
#include <talle/sys/platform.hpp>
#include <talle/style/color.hpp>
#include <talle/style/color_part.hpp>

namespace talle::cmd
{
	struct set_background_color
	{
		set_background_color(const style::color& color) : color{ color } {}

		style::color color;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			out << style::color_part::background{ color };
		}

#ifdef _WIN32
		void call_winapi() const
		{
			sys::set_background_color(color);
		}
#endif
	};
}
