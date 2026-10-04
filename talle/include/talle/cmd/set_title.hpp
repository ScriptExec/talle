#pragma once
#include <talle/meta/writer.hpp>
#include <talle/sys/platform.hpp>

namespace talle::cmd
{
	struct set_title
	{
		set_title(const std::string& title) : title{ title } {}

		std::string title;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			out << "\x1b]0;" << title << "\x07";
		}

#ifdef _WIN32
		void call_winapi() const
		{
			sys::set_title(title);
		}
#endif
	};
}