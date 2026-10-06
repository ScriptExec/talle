#pragma once
#include <talle/cmd/command.hpp>
#include <talle/cmd/reset_attributes.hpp>
#include <talle/sys/platform.hpp>
#include <talle/style/style.hpp>
#include <talle/style/color_part.hpp>

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

			auto rstyle = style.as_reset();
			if (rstyle.underline.has_value())
			{
				out << style::color_part::underline(*rstyle.underline);
			}
			if (rstyle.background.has_value())
			{
				out << style::color_part::background(*rstyle.background);
			}
			if (rstyle.foreground.has_value())
			{
				out << style::color_part::foreground(*rstyle.foreground);
			}
			if (!rstyle.attributes.empty())
			{
				out << cmd::reset_attributes{};
			}
		}
	};
}
