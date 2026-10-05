#include <string>
#include <iostream>
#include <format>
#include <array>
#include <talle/core/terminal.hpp>
#include <talle/core/backend.hpp>

int main(int argc, char* argv[])
{
	using namespace talle;

	auto back = backend{ std::cout };
	auto term = terminal{ back };
	term.set_cursor_visible(false);

	term.write(std::format("{:<10}", "Standard:"));
	for (size_t i = 0; i <= 7; ++i)
	{
		term.set_foreground_color(style::color::base::dark_grey)
			.set_background_color(style::color::ansi(i))
			.write(std::format("{:>3} ", i))
			.reset_foreground_color()
			.reset_background_color();
	}
	term.newline().write(std::string(10, ' '));
	for (size_t i = 0; i <= 7; ++i)
	{
		term.set_foreground_color(style::color::ansi(i))
			.write(std::format("{:>3} ", i))
			.reset_foreground_color();
	}
	term.newline();
	term.write(std::format("{:<10}", "Intense:"));
	for (size_t i = 8; i <= 15; ++i)
	{
		term.set_foreground_color(style::color::base::black)
			.set_background_color(style::color::ansi(i))
			.write(std::format("{:>3} ", i))
			.reset_foreground_color()
			.reset_background_color();
	}
	term.newline().write(std::string(10, ' '));
	for (size_t i = 8; i <= 15; ++i)
	{
		term.set_foreground_color(style::color::ansi(i))
			.write(std::format("{:>3} ", i))
			.reset_foreground_color();
	}

	term.newline(2).write(std::format("{:<10}", "Grays:"));
	for (size_t i = 232; i <= 243; ++i)
	{
		term.set_foreground_color(style::color::base::white)
			.set_background_color(style::color::ansi(i))
			.write(std::format("{:^5}", i))
			.reset_foreground_color()
			.reset_background_color();
	}
	term.newline().write(std::string(10, ' '));
	for (size_t i = 232; i <= 243; ++i)
	{
		term.set_foreground_color(style::color::ansi(i))
			.write(std::format("{:^5}", i))
			.reset_foreground_color();
	}
	term.newline().write(std::string(10, ' '));
	for (size_t i = 244; i <= 255; ++i)
	{
		term.set_foreground_color(style::color::base::black)
			.set_background_color(style::color::ansi(i))
			.write(std::format("{:^5}", i))
			.reset_foreground_color()
			.reset_background_color();
	}
	term.newline().write(std::string(10, ' '));
	for (size_t i = 244; i <= 255; ++i)
	{
		term.set_foreground_color(style::color::ansi(i))
			.write(std::format("{:^5}", i))
			.reset_foreground_color();
	}
	term.newline(2).write(std::format("{:<10}", "Rest:"));
	for (size_t e = 0, i = 16; i <= 231; ++i)
	{
		term.set_foreground_color(style::color::base::black)
			.set_background_color(style::color::ansi(i))
			.write(std::format("{:^5}", i))
			.reset_foreground_color()
			.reset_background_color();
		if (++e == 12 and i != 231)
		{
			e = 0;
			term.newline()
				.write(std::string(10, ' '));
		}
	}
	term.newline().write(std::string(10, ' '));
	for (size_t e = 0, i = 16; i <= 231; ++i)
	{
		term.set_foreground_color(style::color::ansi(i))
			.write(std::format("{:^5}", i))
			.reset_foreground_color();
		if (++e == 12 and i != 231)
		{
			e = 0;
			term.newline()
				.write(std::string(10, ' '));
		}
	}
	term.newline(2).write(std::format("{:<10}", "RGB:"));

	constexpr std::array rgb{ 0, 95, 135, 175, 215, 255	};
	constexpr size_t rgb_cols = 12;
	size_t rgb_col = 0;

	for (size_t r = 0; r < 6; ++r)
	{
		for (size_t g = 0; g < 6; ++g)
		{
			for (size_t b = 0; b < 6; ++b)
			{
				const auto i = 16 + 36 * r + 6 * g + b;

				const auto red = rgb[r];
				const auto green = rgb[g];
				const auto blue = rgb[b];

				const auto brightness = 0.299 * red + 0.587 * green + 0.114 * blue;
				const auto foreground = brightness > 128 ? style::color::base::black : style::color::base::white;

				term.set_foreground_color(foreground)
					.set_background_color(style::color::ansi(i))
					.write(std::format(" {:02X}{:02X}{:02X} ", red, green, blue))
					.reset_foreground_color()
					.reset_background_color();

				if (++rgb_col == rgb_cols)
				{
					rgb_col = 0;
					term.newline()
						.write(std::string(10, ' '));
				}
			}
		}
	}

	for (size_t r = 0; r < 6; ++r)
	{
		for (size_t g = 0; g < 6; ++g)
		{
			for (size_t b = 0; b < 6; ++b)
			{
				const auto i = 16 + 36 * r + 6 * g + b;

				const auto red = rgb[r];
				const auto green = rgb[g];
				const auto blue = rgb[b];

				term.set_foreground_color(style::color::ansi(i))
					.write(std::format(" {:02X}{:02X}{:02X} ", red, green, blue))
					.reset_foreground_color();

				if (++rgb_col == rgb_cols)
				{
					rgb_col = 0;
					term.newline()
						.write(std::string(10, ' '));
				}
			}
		}
	}

	term.set_cursor_visible(true);
	return 0;
}