#pragma once
#include <format>
#include <string_view>
#include <talle/cmd/command.hpp>
#include <talle/style/attribute.hpp>

namespace talle::cmd
{
	struct set_attribute
	{
		set_attribute(style::attribute attr) : attr{ attr } {}

		style::attribute attr;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			std::string_view attr_str;
			switch (attr)
			{
				case style::attribute::reset: attr_str = "0"; break;
				case style::attribute::bold: attr_str = "1"; break;
				case style::attribute::dim: attr_str = "2"; break;
				case style::attribute::italic: attr_str = "3"; break;
				case style::attribute::underlined: attr_str = "4"; break;
				case style::attribute::underlined_double: attr_str = "4:2"; break; //or 21
				case style::attribute::underlined_curled: attr_str = "4:3"; break;
				case style::attribute::underlined_dotted: attr_str = "4:4"; break;
				case style::attribute::underlined_dashed: attr_str = "4:5"; break;
				case style::attribute::slow_blink: attr_str = "5"; break;
				case style::attribute::rapid_blink: attr_str = "6"; break;
				case style::attribute::invert: attr_str = "7"; break;
				case style::attribute::hidden: attr_str = "8"; break;
				case style::attribute::strikethrough: attr_str = "9"; break;
				case style::attribute::framed: attr_str = "51"; break;
				case style::attribute::encircled: attr_str = "52"; break;
				case style::attribute::overlined: attr_str = "53"; break;
				case style::attribute::normal_intensity: attr_str = "22"; break;
				case style::attribute::no_bold: attr_str = "21"; break;
				case style::attribute::no_italic: attr_str = "23"; break;
				case style::attribute::no_underline: attr_str = "24"; break;
				case style::attribute::no_blink: attr_str = "25"; break;
				case style::attribute::no_invert: attr_str = "27"; break;
				case style::attribute::no_hidden: attr_str = "28"; break;
				case style::attribute::no_strikethrough: attr_str = "29"; break;
				case style::attribute::no_frame_or_encircle: attr_str = "54"; break;
				case style::attribute::no_overline: attr_str = "55"; break;
				default: attr_str = "0"; break;
			}
			out << std::format("\x1b[{}m", attr_str);
		}
	};
}

namespace talle::style
{
	template<meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const attribute& attr)
	{
		return writer << cmd::set_attribute{ attr };
	}
}
