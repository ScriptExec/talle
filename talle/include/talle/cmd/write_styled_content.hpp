#pragma once
#include <talle/cmd/command.hpp>
#include <talle/cmd/reset_style.hpp>
#include <talle/sys/platform.hpp>
#include <talle/style/styled.hpp>

namespace talle::cmd
{
	template<meta::content content_type>
	struct write_styled_content
	{
		write_styled_content(const style::styled<content_type>& content) : content{ content } {}

		style::styled<content_type> content;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			const auto& style = content.get_style();
			out << style << content.content() << reset_style{ style };
		}
	};
}

namespace talle::style
{
	template<meta::output_writer writer_type, meta::content content_type>
	writer_type& operator<<(writer_type& writer, const styled<content_type>& content)
	{
		return writer << cmd::write_styled_content{ content };
	}
}