#pragma once
#include <string>
#include <optional>
#include <ostream>

#include <talle/meta/writer.hpp>
#include "color.hpp"
#include "attributes.hpp"

namespace talle::style
{
	struct style
	{
		std::optional<color> foreground;
		std::optional<color> background;
		std::optional<color> underline;
		attributes attributes;

		style& set_foreground(const std::optional<color>& color);
		style& reset_foreground();
		style& set_background(const std::optional<color>& color);
		style& reset_background();
		style& set_underline(const std::optional<color>& color);
		style& reset_underline();

		style& set(attribute attr, bool value);
		style& add(attribute attr);
		style& remove(attribute attr);
		style& reset();

		style as_reset() const;
	};

	template<meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const style& stl);
}
