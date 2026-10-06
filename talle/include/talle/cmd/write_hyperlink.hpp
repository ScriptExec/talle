#pragma once
#include <format>
#include <talle/cmd/command.hpp>
#include <talle/sys/platform.hpp>
#include <talle/style/hyperlink.hpp>

namespace talle::cmd
{
	template<meta::content url_type, meta::content content_type = std::string>
	struct write_hyperlink
	{
		using stored_url_type = meta::content_decay_type<url_type>;
		using stored_content_type = meta::content_decay_type<content_type>;

		write_hyperlink(const style::hyperlink<stored_url_type, stored_content_type>& link) : link{ link } {}

		style::hyperlink<stored_url_type, stored_content_type> link;

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			out << "\x1b]8;;" << link.url() << '\a';
			const auto content = link.content_or_url();
			if constexpr (requires { std::visit([&out](const auto& value) { out << value; }, content); })
			{
				std::visit([&out](const auto& value)
				{
					out << value;
				}, content);
			}
			else
			{
				out << content;
			}
			out << "\x1b]8;;\a";
		}
	};
}

namespace talle::style
{
	template<meta::output_writer writer_type, meta::content url_type, meta::content content_type>
	writer_type& operator<<(writer_type& writer, const hyperlink<url_type, content_type>& link)
	{
		return writer << cmd::write_hyperlink<url_type, content_type>{ link };
	}
}
