#pragma once
#include <format>
#include <talle/cmd/command.hpp>
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
			out << std::format("\x1b]0;{}\x07", title);
		}

#ifdef _WIN32
		void call_winapi() const
		{
			sys::set_title(title);
		}
#endif
	};
}