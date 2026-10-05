#pragma once
#include <ostream>
#include <string>
#include "color_part.hpp"

namespace talle::style
{
	class colored
	{
	public:
		colored(const std::string& text, const color_part& part);

	private:
		std::string text_;
		color_part part_;

	public:
		colored& set_foreground(const color& color);
		colored& set_background(const color& color);
		colored& set_underline(const color& color);

		const std::string& text() const;
		const color_part& get_part() const;
		std::string to_string() const;

		friend std::ostream& operator<<(std::ostream& out, const colored& col);
	};

}
