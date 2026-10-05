#pragma once
#include <talle/meta/writer.hpp>
#include <talle/sys/platform.hpp>
#include <talle/style/color_part.hpp>

namespace talle::cmd
{
	struct set_color
	{
		set_color(const style::color_part& color) : color{ color } {}

		style::color_part color;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			out << color;
		}

#ifdef _WIN32
		void call_winapi() const
		{
			if (auto fg_col = color.try_get<style::color_part::foreground>())
			{
				sys::set_foreground_color(fg_col->value);
			}
			else if (auto bg_col = color.try_get<style::color_part::background>())
			{
				sys::set_background_color(bg_col->value);
			}
			else if (auto ul_col = color.try_get<style::color_part::underline>())
			{
				sys::set_underline_color(ul_col->value);
			}
		}
#endif
	};
}
