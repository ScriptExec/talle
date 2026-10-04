#pragma once
#include <talle/meta/writer.hpp>
#include <talle/sys/platform.hpp>

namespace talle::cmd
{
	struct toggle_cursor_blinking
	{
		toggle_cursor_blinking(bool value) : value{ value } {}

		bool value;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			if (value)
			{
				out << "\x1b[?12h";
			}
			else
			{
				out << "\x1b[?12l";
			}
		}
	};
}
