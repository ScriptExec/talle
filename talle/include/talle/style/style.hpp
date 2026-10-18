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
		///@return A new style that is the result of transitioning to another style, used when optimizing the output of style changes in a terminal
		style transition_into(const style& other) const;

		bool has_foreground() const;
		bool has_background() const;
		bool has_underline() const;
		bool empty() const;

		bool operator==(const style& other) const;
		bool operator!=(const style& other) const;
	};

	template<meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const style& stl);
}

namespace std
{
	template<>
	struct hash<talle::style::style>
	{
		size_t operator()(const talle::style::style& stl) const
		{
			size_t seed = 0;
			seed ^= std::hash<std::optional<talle::style::color>>{}(stl.foreground) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
			seed ^= std::hash<std::optional<talle::style::color>>{}(stl.background) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
			seed ^= std::hash<std::optional<talle::style::color>>{}(stl.underline) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
			seed ^= std::hash<talle::style::attributes>{}(stl.attributes) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}