#pragma once
#include <cstdint>
#include <term/cmd/command.hpp>
#include <term/sys/platform.hpp>
#include <term/utils/position.hpp>

namespace term::cmd
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

		bool is_ansi_supported() const
		{
			return true;
		}
#endif
	};
}
