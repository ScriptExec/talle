#pragma once
#include <term/cmd/command.hpp>
#include <term/sys/platform.hpp>

namespace term::cmd
{
	struct toggle_mouse_capture
	{
		toggle_mouse_capture(bool value) : value{ value } {}

		bool value;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			if (value)
			{
				// Normal tracking: Send mouse X & Y on button press and release
				out << "\x1b[?1000h";
				// Button-event tracking: Report button motion events (dragging)
				out << "\x1b[?1002h";
				// Any-event tracking: Report all motion events
				out << "\x1b[?1003h";
				// RXVT mouse mode: Allows mouse coordinates of >223
				out << "\x1b[?1015h";
				// SGR mouse mode: Allows mouse coordinates of >223, preferred over RXVT mode
				out << "\x1b[?1006h";
			}
			else
			{
				out << "\x1b[?1006l";
				out << "\x1b[?1015l";
				out << "\x1b[?1003l";
				out << "\x1b[?1002l";
				out << "\x1b[?1000l";
			}
		}

#ifdef _WIN32
		void call_winapi() const
		{
			sys::toggle_mouse_capture(value);
		}

		bool is_ansi_supported() const
		{
			return false;
		}
#endif
	};
}
