#pragma once
#include <talle/cmd/command.hpp>
#include <talle/sys/platform.hpp>
#include <talle/style/style.hpp>

namespace talle::cmd
{
	struct reset_style
	{
		reset_style(const style::style& style) : style{ style } {}

		style::style style;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			if (!sys::is_color_enabled()) return;
			out << style.to_reset_string();
		}

#ifdef _WIN32
		void call_winapi() const
		{
			if (!sys::is_color_enabled()) return;
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
