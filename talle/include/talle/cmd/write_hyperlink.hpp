#pragma once
#include <format>
#include <talle/cmd/command.hpp>
#include <talle/sys/platform.hpp>
#include <talle/style/hyperlink.hpp>

namespace talle::cmd
{
	struct write_hyperlink
	{
		write_hyperlink(const style::hyperlink& link) : link{ link } {}

		style::hyperlink link;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			out << std::format("\x1b]8;;{}\a{}\x1b]8;;\a", link.url(), link.content_or_url());
		}
	};
}

namespace talle::style
{
	template<meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const hyperlink& link)
	{
		return writer << cmd::write_hyperlink{ link };
	}
}
