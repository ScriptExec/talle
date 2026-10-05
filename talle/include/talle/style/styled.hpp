#pragma once
#include <string>
#include "style.hpp"

namespace talle::style
{
	class styled
	{
	public:
		styled(const std::string& text, const style& style);

	private:
		std::string text_;
		style style_;

	public:
		styled& set_foreground(const std::optional<color>& color);
		styled& reset_foreground();
		styled& set_background(const std::optional<color>& color);
		styled& reset_background();
		styled& set_underline(const std::optional<color>& color);
		styled& reset_underline();

		const std::string& text() const;
		const style& get_style() const;
		std::string to_string() const;
		friend std::ostream& operator<<(std::ostream& out, const styled& col);
	};
}
