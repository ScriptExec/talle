#pragma once
#include <talle/meta/writer.hpp>
#include <talle/sys/platform.hpp>
#include <talle/utils/position.hpp>

namespace talle::cmd
{
	struct move_cursor
	{
		move_cursor() = default;
		move_cursor(position pos) : pos{ pos } {}

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
