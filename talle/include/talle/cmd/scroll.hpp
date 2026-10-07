#pragma once
#include <cstdint>
#include <format>
#include <talle/cmd/command.hpp>
#include <talle/sys/platform.hpp>
#include <talle/input/scroll.hpp>

namespace talle::cmd
{
	struct scroll
	{
		scroll(scroll_direction direction, uint16_t rows) : direction{ direction }, rows{ rows } {}

		scroll_direction direction;
		uint16_t rows;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			if (rows == 0) return;
			switch (direction)
			{
				case scroll_direction::up: out << std::format("\x1b[?{}S", rows); break;
				case scroll_direction::down: out << std::format("\x1b[?{}T", rows); break;
				default: break;
			}
		}

#ifdef _WIN32
		void call_winapi() const
		{
			if (rows == 0) return;
			sys::scroll(direction, rows);
		}
#endif
	};
}
