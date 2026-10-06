#pragma once
#include <talle/cmd/command.hpp>
#include <talle/sys/platform.hpp>

namespace talle::cmd
{
	struct clear
	{
		clear(clear_type type) : type{ type } {}

		clear_type type;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			switch (type)
			{
				case clear_type::all: out << "\x1b[2J"; break;
				case clear_type::all_and_history: out << "\x1b[3J"; break;
				case clear_type::from_cursor_down: out << "\x1b[0J"; break;
				case clear_type::from_cursor_up: out << "\x1b[1J"; break;
				case clear_type::from_cursor_to_start: out << "\x1b[1K"; break;
				case clear_type::current_line: out << "\x1b[2K"; break;
				case clear_type::until_newline: out << "\x1b[K"; break;
			}
		}

#ifdef _WIN32
		void call_winapi() const
		{
			sys::clear(type);
		}

		bool is_ansi_supported() const
		{
			return false;
		}
#endif
	};
}
