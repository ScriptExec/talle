#pragma once
#include <talle/meta/writer.hpp>
#include <talle/sys/platform.hpp>
#include <talle/utils/size.hpp>

namespace talle::cmd
{
	struct set_size
	{
		set_size(size new_size) : new_size{ new_size } {}

		size new_size;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			out << "\x1b[8;" << new_size.height << ";" << new_size.width << "t";
		}

#ifdef _WIN32
		void call_winapi() const
		{
			sys::set_size(new_size);
		}
#endif
	};
}