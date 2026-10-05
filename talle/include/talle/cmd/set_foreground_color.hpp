#pragma once
#include <talle/meta/writer.hpp>
#include <talle/sys/platform.hpp>
#include <talle/style/color.hpp>
#include <talle/style/color_part.hpp>

namespace talle::cmd
{
	struct set_foreground_color
	{
		set_foreground_color(const style::color& color) : color{ color } {}

		style::color color;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			out << style::color_part::foreground{ color };
		}

#ifdef _WIN32
		void call_winapi() const
		{
			sys::set_foreground_color(color);
		}
#endif
	};
}
