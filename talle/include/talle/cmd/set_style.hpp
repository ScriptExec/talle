#pragma once
#include <talle/meta/writer.hpp>
#include <talle/sys/platform.hpp>
#include <talle/style/style.hpp>

namespace talle::cmd
{
	struct set_style
	{
		set_style(const style::style& style) : style{ style } {}

		style::style style;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			out << style;
		}

#ifdef _WIN32
		void call_winapi() const
		{
			if (style.foreground.has_value())
			{
				sys::set_foreground_color(*style.foreground);
			}
			if (style.background.has_value())
			{
				sys::set_background_color(*style.background);
			}
			if (style.underline.has_value())
			{
				sys::set_underline_color(*style.underline);
			}
		}
#endif
	};
}
