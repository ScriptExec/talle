#include <talle/style/styled.hpp>
#include <talle/sys/platform.hpp>

namespace talle::style
{
	styled::styled(const std::string& content, const style& style) : content_{ content }, style_{ style } {}

	styled& styled::set_foreground(const std::optional<color>& color)
	{
		style_.set_foreground(color);
		return *this;
	}

	styled& styled::reset_foreground()
	{
		style_.reset_foreground();
		return *this;
	}

	styled& styled::set_background(const std::optional<color>& color)
	{
		style_.set_background(color);
		return *this;
	}

	styled& styled::reset_background()
	{
		style_.reset_background();
		return *this;
	}

	styled& styled::set_underline(const std::optional<color>& color)
	{
		style_.set_underline(color);
		return *this;
	}

	styled& styled::reset_underline()
	{
		style_.reset_underline();
		return *this;
	}

	const std::string& styled::content() const
	{
		return content_;
	}

	const style& styled::get_style() const
	{
		return style_;
	}

	std::string styled::to_string() const
	{
		if (!sys::is_color_enabled()) return content_;
		auto reset_part = style_.as_reset();
		return style_.to_string() + content_ + reset_part.to_string();
	}
}
