#pragma once
#include <talle/meta/writer.hpp>
#include <talle/sys/platform.hpp>

namespace talle::cmd
{
	struct save_cursor_pos
	{
		save_cursor_pos() = default;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			out << "\x1b[s";
		}

#ifdef _WIN32
		void call_winapi() const
		{
			sys::save_cursor_pos();
		}
#endif
	};
}
