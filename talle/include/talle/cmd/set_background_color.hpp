#pragma once
#include <talle/cmd/command.hpp>
#include <talle/sys/platform.hpp>
#include <talle/style/color.hpp>
#include <talle/style/color_part.hpp>

namespace talle::cmd
{
	struct set_background_color
	{
		set_background_color(const style::color& color) : color{ color } {}

		style::color color;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			if (!sys::is_color_enabled()) return;
			auto col = style::color_part::background{ color };
			out << "\x1b[" + col.to_string() + "m";
		}

#ifdef _WIN32
		void call_winapi() const
		{
			if (!sys::is_color_enabled()) return;
			sys::set_background_color(color);
		}
#endif
	};
}

namespace talle::style
{
	template<meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const color_part::background& bg)
	{
		return writer << talle::cmd::set_background_color{ bg.value };
	}
}
