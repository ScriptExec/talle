#pragma once
#include <talle/cmd/command.hpp>
#include <talle/sys/platform.hpp>
#include <talle/style/colored.hpp>

namespace talle::cmd
{
	struct write_colored_content
	{
		write_colored_content(const style::colored& content) : content{ content } {}

		style::colored content;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			return out << content.to_string();
		}
	};
}

namespace talle::style
{
	template<meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const colored& content)
	{
		return writer << cmd::write_colored_content{ content };
	}
}