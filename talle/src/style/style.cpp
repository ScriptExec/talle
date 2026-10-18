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

	style style::transition_into(const style& other) const
	{
		style result;
		
		result.foreground = (foreground == other.foreground) ? std::nullopt : std::optional{ other.foreground.value_or(color::reset()) };
		result.background = (background == other.background) ? std::nullopt : std::optional{ other.background.value_or(color::reset()) };
		result.underline = (underline == other.underline) ? std::nullopt : std::optional{ other.underline.value_or(color::reset()) };

		if (attributes != other.attributes)
		{
			result.attributes = other.attributes;
			/*
			if (attributes.has(attribute::bold) and !other.attributes.has(attribute::bold))
			{
				result.attributes.add(attribute::no_bold);
			}
			if (attributes.has(attribute::italic) and !other.attributes.has(attribute::italic))
			{
				result.attributes.add(attribute::no_italic);
			}
			auto other_underlines = { attribute::underlined, attribute::underlined_double, attribute::underlined_curled, attribute::underlined_dotted, attribute::underlined_dashed };
			if (attributes.has(other_underlines) and !other.attributes.has(other_underlines))
			{
				result.attributes.add(attribute::no_underline);
			}
			if (attributes.has({ attribute::slow_blink, attribute::rapid_blink }) and !other.attributes.has({ attribute::slow_blink, attribute::rapid_blink }))
			{
				result.attributes.add(attribute::no_blink);
			}
			if (attributes.has(attribute::invert) and !other.attributes.has(attribute::invert))
			{
				result.attributes.add(attribute::no_invert);
			}
			if (attributes.has(attribute::hidden) and !other.attributes.has(attribute::hidden))
			{
				result.attributes.add(attribute::no_hidden);
			}
			if (attributes.has(attribute::strikethrough) and !other.attributes.has(attribute::strikethrough))
			{
				result.attributes.add(attribute::no_strikethrough);
			}
			if (attributes.has({ attribute::framed, attribute::encircled }) and !other.attributes.has({ attribute::framed, attribute::encircled }))
			{
				result.attributes.add(attribute::no_frame_or_encircle);
			}
			if (attributes.has(attribute::overlined) and !other.attributes.has(attribute::overlined))
			{
				result.attributes.add(attribute::no_overline);
			}
			*/
		}
		return result;
	}

	bool style::has_foreground() const
	{
		return foreground.has_value();
	}

	bool style::has_background() const
	{
		return background.has_value();
	}

	bool style::has_underline() const
	{
		return underline.has_value();
	}

	bool style::empty() const
	{
		return !foreground.has_value() and !background.has_value() and !underline.has_value() and attributes.empty();
	}

	bool style::operator==(const style& other) const
	{
		return foreground == other.foreground and
			background == other.background and
			underline == other.underline and
			attributes == other.attributes;
	}

	bool style::operator!=(const style& other) const
	{
		return !(*this == other);
	}
}
