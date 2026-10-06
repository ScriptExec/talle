#pragma once
#include <talle/cmd/command.hpp>
#include <talle/sys/platform.hpp>
#include <talle/style/style.hpp>
#include <talle/style/color_part.hpp>

namespace talle::cmd
{
	struct set_style
	{
		set_style(const style::style& style) : style{ style } {}

		style::style style;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			if (!sys::is_color_enabled()) return;

			if (style.foreground.has_value())
			{
				out << style::color_part::foreground(*style.foreground);
			}
			if (style.background.has_value())
			{
				out << style::color_part::background(*style.background);
			}
			if (style.underline.has_value())
			{
				out << style::color_part::underline(*style.underline);
			}

			if (!style.attributes.empty())
			{
				out << style.attributes;
			}
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

namespace talle::style
{
	template<meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const style& stl)
	{
		return writer << talle::cmd::set_style{ stl };
	}
}
