#include <talle/style/colored.hpp>
#include <talle/sys/platform.hpp>

namespace talle::style
{
	colored::colored(const std::string& text, const color_part& part) : text_{ text }, part_{ part } {}

	colored& colored::set_foreground(const color& color)
	{
		part_ = color_part::foreground{ color };
		return *this;
	}

	colored& colored::set_background(const color& color)
	{
		part_ = color_part::background{ color };
		return *this;
	}

	colored& colored::set_underline(const color& color)
	{
		part_ = color_part::underline{ color };
		return *this;
	}

	const std::string& colored::text() const
	{
		return text_;
	}

	const color_part& colored::get_part() const
	{
		return part_;
	}

	std::string colored::to_string() const
	{
		if (!sys::is_color_enabled()) return text_;
		auto reset_part = part_.as_reset();
		return part_.to_string() + text_ + reset_part.to_string();
	}

	std::ostream& operator<<(std::ostream& out, const colored& col)
	{
		if (!sys::is_color_enabled()) return out << col.text();
		auto reset_part = col.get_part().as_reset();
		return out << col.get_part() << col.text() << reset_part;
	}
}
