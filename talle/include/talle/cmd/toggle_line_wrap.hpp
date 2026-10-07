#pragma once
#include <talle/cmd/command.hpp>
#include <talle/sys/platform.hpp>

namespace talle::cmd
{
	struct toggle_line_wrap
	{
		toggle_line_wrap(bool value) : value{ value } {}

		bool value;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			if (value)
			{
				out << "\x1b[?7h";
			}
			else
			{
				out << "\x1b[?7l";
			}
		}

#ifdef _WIN32
		void call_winapi() const
		{
			sys::toggle_line_wrap(value);
		}
#endif
	};
}
