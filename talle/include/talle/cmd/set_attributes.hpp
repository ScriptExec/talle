#pragma once
#include <format>
#include <string_view>
#include <talle/cmd/command.hpp>
#include <talle/cmd/set_attribute.hpp>
#include <talle/style/attributes.hpp>

namespace talle::cmd
{
	struct set_attributes
	{
		set_attributes(const style::attributes& attrs) : attrs{ attrs } {}

		style::attributes attrs;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			for (auto [attr, value] : attrs)
			{
				if (value)
				{
					out << cmd::set_attribute{ attr };
				}
			}
		}
	};
}

namespace talle::style
{
	template<meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const attributes& attr)
	{
		return writer << cmd::set_attributes{ attr };
	}
}
