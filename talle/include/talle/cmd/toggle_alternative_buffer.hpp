#pragma once
#include <talle/meta/writer.hpp>
#include <talle/sys/platform.hpp>

namespace talle::cmd
{
	struct toggle_alternative_buffer
	{
		toggle_alternative_buffer(bool value) : value{ value } {}

		bool value;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			if (value)
			{
				out << "\x1b[?1049h";
			}
			else
			{
				out << "\x1b[?1049l";
			}
		}

#ifdef _WIN32
		void call_winapi() const
		{
			sys::toggle_alternative_buffer(value);
		}
#endif
	};
}
