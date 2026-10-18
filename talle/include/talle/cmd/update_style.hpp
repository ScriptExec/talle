#pragma once
#include <talle/cmd/command.hpp>
#include <talle/sys/platform.hpp>
#include <talle/style/style.hpp>
#include <talle/style/color_part.hpp>

namespace talle::cmd
{
	struct update_style
	{
		update_style(const style::style& prev_style, const style::style& next_style) : prev_style{ prev_style }, next_style{ next_style } {}

		style::style prev_style;
		style::style next_style;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			if (!sys::is_color_enabled()) return;

			auto style = prev_style.transition_into(next_style);

			if (prev_style.attributes != next_style.attributes)
			{
				out << cmd::reset_attributes{};
				if (!style.attributes.empty())
				{
					out << style.attributes;
				}
			}

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
		}

#ifdef _WIN32
		void call_winapi() const
		{
			if (!sys::is_color_enabled()) return;

			auto style = prev_style.transition_into(next_style);

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
