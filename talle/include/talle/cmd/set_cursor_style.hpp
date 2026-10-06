#pragma once
#include <talle/cmd/command.hpp>
#include <talle/sys/platform.hpp>
#include <talle/input/cursor_style.hpp>

namespace talle::cmd
{
	struct set_cursor_style
	{
		set_cursor_style(cursor_style style) : style{ style } {}

		cursor_style style;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			switch (style)
			{
				case cursor_style::user_default: out << "\x1b[0 q"; break;
				case cursor_style::blinking_block: out << "\x1b[1 q"; break;
				case cursor_style::steady_block: out << "\x1b[2 q"; break;
				case cursor_style::blinking_underline: out << "\x1b[3 q"; break;
				case cursor_style::steady_underline: out << "\x1b[4 q"; break;
				case cursor_style::blinking_bar: out << "\x1b[5 q"; break;
				case cursor_style::steady_bar: out << "\x1b[6 q"; break;
			}
		}
	};
}
