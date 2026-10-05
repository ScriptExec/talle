#pragma once
#include <string>
#include <optional>
#include <ostream>

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

		friend std::ostream& operator<<(std::ostream& out, const style& style);
	};
}
