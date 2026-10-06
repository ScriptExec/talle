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

	style& style::set(attribute attr, bool value)
	{
		attributes.set(attr, value);
		return *this;
	}

	style& style::add(attribute attr)
	{
		attributes.add(attr);
		return *this;
	}

	style& style::remove(attribute attr)
	{
		attributes.remove(attr);
		return *this;
	}

	style& style::reset()
	{
		attributes.reset();
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
		if (!attributes.empty())
		{
			result.attributes = { attribute::reset };
		}
		return result;
	}
}
