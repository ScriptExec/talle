#pragma once
#include <string>
#include <cstdio>
#include <cstdint>
#include <optional>
#include <iostream>
#include <csignal>
#include <limits>

#include <fcntl.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/ioctl.h>
#include <termios.h>

#include <talle/utils/position.hpp>
#include <talle/event/event.hpp>
#include <talle/style/color.hpp>
#include <talle/input/scroll.hpp>

namespace talle::sys
{
	struct platform_data
	{

	} data;

	std::optional<handle> stdout_handle()
	{
		return reinterpret_cast<handle>(STDOUT_FILENO);
	}

	std::optional<handle> stderr_handle()
	{
		return reinterpret_cast<handle>(STDERR_FILENO);
	}

	std::optional<handle> stdin_handle()
	{
		return reinterpret_cast<handle>(STDIN_FILENO);
	}

	std::optional<handle> current_output_handle()
	{
		int fd = open("/dev/tty", O_WRONLY);
		if (fd == -1) return std::nullopt;
		return reinterpret_cast<handle>(fd);
	}

	std::optional<handle> current_input_handle()
	{
		int fd = open("/dev/tty", O_RDONLY);
		if (fd == -1) return std::nullopt;
		return reinterpret_cast<handle>(fd);
	}

	bool setup()
	{
		return true;
	}

	bool set_size(size new_size)
	{
		return false;
	}

	std::optional<size> get_window_size()
	{
		auto handle = current_output_handle();
		if (!handle.has_value())
		{
			handle = stdout_handle();
		}
		if (!handle.has_value()) return std::nullopt;
		auto fd = reinterpret_cast<int>(*handle);
		auto wsize = winsize{};
		if (ioctl(fd, TIOCGWINSZ, &wsize) == -1) return std::nullopt;
		return size{ static_cast<uint16_t>(wsize.ws_col), static_cast<uint16_t>(wsize.ws_row) };
	}

	std::optional<uint16_t> tput_value(const char* value)
	{
		std::string command = "tput ";
		command += value;

		FILE* pipe = popen(command.c_str(), "r");
		if (!pipe) return std::nullopt;

		char buffer[64]{};

		if (!std::fgets(buffer, sizeof(buffer), pipe))
		{
			pclose(pipe);
			return std::nullopt;
		}

		int status = pclose(pipe);
		if (status != 0) return std::nullopt;

		char* end = nullptr;
		errno = 0;

		long result = std::strtol(buffer, &end, 10);

		if (errno != 0 or end == buffer or result < 0 or result > std::numeric_limits<uint16_t>::max()) return std::nullopt;
		return static_cast<uint16_t>(result);
	}

	std::optional<size> get_size()
	{
		auto size_opt = get_window_size();
		if (size_opt.has_value()) return size_opt;

		auto width = tput_value("cols");
		auto height = tput_value("lines");

		if (!width.has_value() or !height.has_value()) return std::nullopt;
		return size{ *width, *height };
	}

	bool toggle_alternative_buffer(bool value)
	{
		return false;
	}

	bool toggle_mouse_capture(bool value)
	{
		return false;
	}

	bool toggle_line_wrap(bool value)
	{
		return false;
	}

	bool scroll(scroll_direction direction, uint16_t rows)
	{
		return false;
	}

	bool set_raw_mode(bool value)
	{
		struct termios tty{};
		
		if (tcgetattr(STDIN_FILENO, &tty) != 0) return false;
		if (value)
		{
			tty.c_iflag &= ~(IXON | ICRNL);
			tty.c_lflag &= ~(ICANON | ECHO | ISIG);
			tty.c_cc[VMIN] = 0;
			tty.c_cc[VTIME] = 1;
		}
		else
		{
			tty.c_iflag |= (IXON | ICRNL);
			tty.c_lflag |= (ICANON | ECHO | ISIG);
			tty.c_cc[VMIN] = 1;
			tty.c_cc[VTIME] = 0;
		}
		return tcsetattr(STDIN_FILENO, TCSANOW, &tty) == 0;
	}

	bool is_raw_mode_enabled()
	{
		struct termios tty {};

		if (tcgetattr(STDIN_FILENO, &tty) != 0) return false;
		return !(tty.c_iflag & (IXON | ICRNL)) and !(tty.c_lflag & (ICANON | ECHO | ISIG));
	}

	bool set_cursor_visible(bool value)
	{
		return false;
	}

	bool set_cursor_pos(position pos)
	{
		return false;
	}

	bool set_title(const std::string& title)
	{
		return false;
	}

	std::optional<position> read_cursor_pos_raw()
	{
		std::cout << "\x1b[6n" << std::flush;

		std::string response;
		response.reserve(32);
		char c{};
		while (std::cin.get(c))
		{
			response.push_back(c);

			if (c == 'R') break;
			if (response.size() >= 64) return std::nullopt;
		}

		if (response.empty() or response.back() != 'R') return std::nullopt;

		// Expected response: ESC [ row ; column R
		if (response.size() < 6 or response[0] != '\x1b' or response[1] != '[') return std::nullopt;

		const auto semicolon = response.find(';', 2);
		if (semicolon == std::string::npos) return std::nullopt;

		const auto terminator = response.find('R', semicolon + 1);
		if (terminator == std::string::npos) return std::nullopt;

		try
		{
			const int row = std::stoi(response.substr(2, semicolon - 2));
			auto column_str = response.substr(semicolon + 1, terminator - semicolon - 1);
			const int column = std::stoi(column_str);

			if (row <= 0 or column <= 0) return std::nullopt;

			return position{ column, row };
		}
		catch (const std::exception& ex)
		{
			return std::nullopt;
		}
		return std::nullopt;
	}

	std::optional<position> read_cursor_pos()
	{
		set_raw_mode(true);
		auto result = read_cursor_pos_raw();
		set_raw_mode(false);
		return result;
	}

	std::optional<position> get_cursor_pos()
	{
		return is_raw_mode_enabled() ? read_cursor_pos_raw() : read_cursor_pos();
	}

	std::optional<event> read_event()
	{
		return std::nullopt;
	}

	bool set_foreground_color(const style::color& color)
	{
		return false;
	}

	bool set_background_color(const style::color& color)
	{
		return false;
	}

	bool set_underline_color(const style::color& color)
	{
		return false;
	}
}
