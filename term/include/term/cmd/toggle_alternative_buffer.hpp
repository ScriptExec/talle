#pragma once
#include <term/cmd/command.hpp>
#include <term/sys/platform.hpp>

namespace term::cmd
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

		bool is_ansi_supported() const
		{
			return true;
		}
#endif
	};
}
