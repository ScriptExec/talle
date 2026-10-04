#pragma once
#include <talle/meta/writer.hpp>
#include <talle/sys/platform.hpp>

namespace talle::cmd
{
	struct set_cursor_visible
	{
		set_cursor_visible(bool value) : value{ value } {}

		bool value;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			if (value)
			{
				out << "\x1b[?25h";
			}
			else
			{
				out << "\x1b[?25l";
			}
		}

#ifdef _WIN32
		void call_winapi() const
		{
			sys::set_cursor_visible(value);
		}
#endif
	};
}
