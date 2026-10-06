#include <talle/style/colored.hpp>
#include <talle/sys/platform.hpp>

namespace talle::style
{
	colored::colored(const std::string& content, const color_part& part) : content_{ content }, part_{ part } {}

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

	const std::string& colored::content() const
	{
		return content_;
	}

	const color_part& colored::get_part() const
	{
		return part_;
	}

	std::string colored::to_string() const
	{
		if (!sys::is_color_enabled()) return content_;
		auto reset_part = part_.as_reset();
		return part_.to_string() + content_ + reset_part.to_string();
	}
}
