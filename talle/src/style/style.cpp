#include <talle/style/style.hpp>
#include <talle/style/color_part.hpp>
#include <talle/sys/platform.hpp>

namespace talle::style
{
	style& style::set_foreground(const std::optional<color>& color)
	{
		foreground = color;
		return *this;
	}
	style& style::reset_foreground()
	{
		foreground.reset();
		return *this;
	}
	style& style::set_background(const std::optional<color>& color)
	{
		background = color;
		return *this;
	}
	style& style::reset_background()
	{
		background.reset();
		return *this;
	}
	style& style::set_underline(const std::optional<color>& color)
	{
		underline = color;
		return *this;
	}
	style& style::reset_underline()
	{
		underline.reset();
		return *this;
	}

	style style::as_reset() const
	{
		style result;
		if (foreground.has_value())
		{
			result.foreground = color::reset();
		}
		if (background.has_value())
		{
			result.background = color::reset();
		}
		if (underline.has_value())
		{
			result.underline = color::reset();
		}
		return result;
	}

	std::string style::to_string() const
	{
		std::string result;
		if (foreground.has_value())
		{
			result += color_part::foreground(*foreground).to_string();
		}
		if (background.has_value())
		{
			result += color_part::background(*background).to_string();
		}
		if (underline.has_value())
		{
			result += color_part::underline(*underline).to_string();
		}
		return result;
	}

	std::string style::to_reset_string() const
	{
		std::string result;
		auto reset_style = as_reset();
		if (reset_style.underline.has_value())
		{
			result += color_part::underline(*reset_style.underline).to_string();
		}
		if (reset_style.background.has_value())
		{
			result += color_part::background(*reset_style.background).to_string();
		}
		if (reset_style.foreground.has_value())
		{
			result += color_part::foreground(*reset_style.foreground).to_string();
		}
		return result;
	}

	std::ostream& operator<<(std::ostream& out, const style& style)
	{
		if (!sys::is_color_enabled()) return out;
		out << style.to_string();
		return out;
	}
}
