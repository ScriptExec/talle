#include <string>
#include <iostream>
#include <format>
#include <array>
#include <string_view>
#include <talle/talle.hpp>
#include <talle/style/hsv.hpp>

using namespace talle;

std::string_view map_attribute_name(style::attribute attr)
{
	switch (attr)
	{
		case style::attribute::reset: return "Reset";
		case style::attribute::bold: return "Bold";
		case style::attribute::dim: return "Dim";
		case style::attribute::italic: return "Italic";
		case style::attribute::underlined: return "Underlined";
		case style::attribute::underlined_double: return "Underlined Double";
		case style::attribute::underlined_curled: return "Underlined Curled";
		case style::attribute::underlined_dotted: return "Underlined Dotted";
		case style::attribute::underlined_dashed: return "Underlined Dashed";
		case style::attribute::slow_blink: return "Slow Blink";
		case style::attribute::rapid_blink: return "Rapid Blink";
		case style::attribute::invert: return "Invert";
		case style::attribute::hidden: return "Hidden";
		case style::attribute::strikethrough: return "Strikethrough";
		case style::attribute::framed: return "Framed";
		case style::attribute::encircled: return "Encircled";
		case style::attribute::overlined: return "Overlined";
		case style::attribute::normal_intensity: return "Normal Intensity";
		default: return "";
	}
}

int main(int argc, char* argv[])
{
	constexpr std::string_view text = "abcdefghijklmnopqrstuvwxyz01234567890.:,;'\"()[]!?+-*/=";
	constexpr size_t text_size = text.size();

	auto attributes = std::array
	{
		style::attribute::reset,
		style::attribute::bold,
		style::attribute::dim,
		style::attribute::italic,
		style::attribute::underlined,
		style::attribute::underlined_double,
		style::attribute::underlined_curled,
		style::attribute::underlined_dotted,
		style::attribute::underlined_dashed,
		style::attribute::slow_blink,
		style::attribute::rapid_blink,
		style::attribute::invert,
		style::attribute::hidden,
		style::attribute::strikethrough,
		style::attribute::framed,
		style::attribute::encircled,
		style::attribute::overlined,
		style::attribute::normal_intensity,
	};

	auto back = backend{ std::cout };
	auto term = terminal{ back };
	term.set_cursor_visible(false);

	for (auto attr : attributes)
	{
		term.write(std::format("{:<22}", std::format("{}: ", map_attribute_name(attr))))
			.set_attribute(attr);

		for (size_t w = 0; auto c : text)
		{
			const auto color = style::hsv_to_rgb({ static_cast<float>((360.0 / text_size) * w++), 1.0, 1.0 });

			term.set_foreground_color(color)
				.write(c)
				.flush();
		}
		term.reset_foreground_color()
			.reset_attribute()
			.newline();
	}
	term.newline();

	constexpr std::string_view url = "https://github.com/ScriptExec/talle";
	term.write(std::format("{:<22}", "Hyperlink (Url): "))
		.hyperlink({ url })
		.newline()
		.write(std::format("{:<22}", "Hyperlink (Content): "))
		.hyperlink({ "TALL-E's GitHub", url })
		.flush();

	term.newline();

	term.set_cursor_visible(true);
	return 0;
}