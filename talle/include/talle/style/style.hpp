#pragma once
#include <string>
#include <optional>
#include <ostream>

#include <talle/meta/writer.hpp>
#include "color.hpp"

namespace talle::style
{
	struct style
	{
		std::optional<color> foreground;
		std::optional<color> background;
		std::optional<color> underline;

		style& set_foreground(const std::optional<color>& color);
		style& reset_foreground();
		style& set_background(const std::optional<color>& color);
		style& reset_background();
		style& set_underline(const std::optional<color>& color);
		style& reset_underline();

		style as_reset() const;
		std::string to_string() const;
		std::string to_reset_string() const;
	};

	template<meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const style& stl);
}
