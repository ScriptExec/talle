#pragma once
#include <talle/cmd/command.hpp>
#include <talle/sys/platform.hpp>
#include <talle/style/styled.hpp>

namespace talle::cmd
{
	struct write_styled_content
	{
		write_styled_content(const style::styled& content) : content{ content } {}

		style::styled content;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			const auto& style = content.get_style();
			return out << style << content.content() << style.to_reset_string();
		}
	};
}

namespace talle::style
{
	template<meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const styled& content)
	{
		return writer << cmd::write_styled_content{ content };
	}
}