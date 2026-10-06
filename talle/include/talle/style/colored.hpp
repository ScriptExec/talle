#pragma once
#include <ostream>
#include <string>
#include "color_part.hpp"

namespace talle::style
{
	class colored
	{
	public:
		colored(const std::string& content, const color_part& part);

	private:
		std::string content_;
		color_part part_;

	public:
		colored& set_foreground(const color& color);
		colored& set_background(const color& color);
		colored& set_underline(const color& color);

		const std::string& content() const;
		const color_part& get_part() const;
		std::string to_string() const;
	};

}

namespace talle::style
{
	template<meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const colored& content);
}
