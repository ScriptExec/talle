#pragma once
#include <talle/cmd/command.hpp>
#include <talle/sys/platform.hpp>
#include <talle/utils/position.hpp>

namespace talle::cmd
{
	struct set_cursor_pos
	{
		set_cursor_pos() = default;
		set_cursor_pos(position pos) : pos{ pos } {}

		position pos;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			out << "\x1b[" << pos.y + 1 << ";" << pos.x + 1 << "H";
		}

#ifdef _WIN32
		void call_winapi() const
		{
			sys::set_cursor_pos(pos);
		}
#endif
	};
}
