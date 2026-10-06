#pragma once
#include <string>
#include "style.hpp"

namespace talle::style
{
	class styled
	{
	public:
		styled(const std::string& content, const style& style);

	private:
		std::string content_;
		style style_;

	public:
		styled& set_foreground(const std::optional<color>& color);
		styled& reset_foreground();
		styled& set_background(const std::optional<color>& color);
		styled& reset_background();
		styled& set_underline(const std::optional<color>& color);
		styled& reset_underline();

		const std::string& content() const;
		const style& get_style() const;
		std::string to_string() const;
		
	};

	template<meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const styled& content);
}
