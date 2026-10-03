#pragma once
#include <cstdint>
#include <optional>

#define VC_EXTRALEAN
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <term/utils/position.hpp>
#include <term/event/event.hpp>
#include <term/event/resize_event.hpp>

namespace term::sys
{
	std::optional<HANDLE> get_current_output_handle()
	{
		HANDLE handle = CreateFile
		(
			"CONOUT$", GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr, OPEN_EXISTING, 0, nullptr
		);
		if (handle == INVALID_HANDLE_VALUE) return std::nullopt;
		return handle;
	}

	std::optional<HANDLE> get_current_input_handle()
	{
		HANDLE handle = CreateFile
		(
			"CONIN$", GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr, OPEN_EXISTING, 0, nullptr
		);
		if (handle == INVALID_HANDLE_VALUE) return std::nullopt;
		return handle;
	}

	bool setup()
	{
		auto handle = get_current_output_handle();
		if (!handle) return false;
		DWORD mode;
		if (!SetConsoleOutputCP(CP_UTF8)) return false;
		if (!GetConsoleMode(*handle, &mode)) return false;
		mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		return SetConsoleMode(*handle, mode);
	}

	bool set_cursor_visible(bool value)
	{
		auto handle = get_current_output_handle();
		if (!handle) return false;

		CONSOLE_CURSOR_INFO cci;
		GetConsoleCursorInfo(*handle, &cci);
		cci.bVisible = value;
		SetConsoleCursorInfo(*handle, &cci);
		return true;
	}

	bool set_cursor_pos(position pos)
	{
		auto handle = get_current_output_handle();
		if (!handle) return false;
		COORD coord;
		coord.X = pos.x;
		coord.Y = pos.y;
		return SetConsoleCursorPosition(*handle, coord);
	}

	bool toggle_alternative_buffer(bool value)
	{
		auto handle = get_current_output_handle();
		if (!handle) return false;

		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = nullptr;
		sa.bInheritHandle = TRUE;
		auto new_handle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, CONSOLE_TEXTMODE_BUFFER, nullptr);
		if (new_handle == INVALID_HANDLE_VALUE) return false;
		return SetConsoleActiveScreenBuffer(new_handle);
	}

	bool toggle_mouse_capture(bool value)
	{
		auto handle = get_current_input_handle();
		if (!handle) return false;
		DWORD mode;
		if (!GetConsoleMode(*handle, &mode)) return false;
		if (value)
		{
			mode |= ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT;
			mode &= ~ENABLE_QUICK_EDIT_MODE;
		}
		else
		{
			mode &= ~(ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT);
		}
		return SetConsoleMode(*handle, mode);
	}

	std::optional<position> get_cursor_pos()
	{
		//read it from dwCursorPosition
		auto handle = get_current_output_handle();
		if (!handle) return std::nullopt;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(*handle, &csbi)) return std::nullopt;

		uint16_t x = static_cast<uint16_t>(csbi.dwCursorPosition.X);
		uint16_t y = static_cast<uint16_t>(csbi.dwCursorPosition.Y);

		auto window_size = csbi.srWindow;
		auto terminal_width = csbi.srWindow.Right - csbi.srWindow.Left;
		auto terminal_height = csbi.srWindow.Bottom - csbi.srWindow.Top;

		if (y > terminal_height)
		{
			y -= window_size.Top;
		}
		return position{ x, y };
	}

	uint32_t fill_with_char(HANDLE handle, COORD start_pos, uint32_t length, char fill_char)
	{
		DWORD chars_written;
		if (!FillConsoleOutputCharacterA(handle, fill_char, length, start_pos, &chars_written)) return 0;
		return chars_written;
	}

	uint32_t fill_with_attribute(HANDLE handle, COORD start_pos, uint32_t length, uint16_t attribute)
	{
		DWORD chars_written;
		if (!FillConsoleOutputAttribute(handle, attribute, length, start_pos, &chars_written)) return 0;
		return chars_written;
	}

	bool clear(COORD start_pos, uint32_t length, uint16_t attribute)
	{
		auto handle = get_current_output_handle();
		if (!handle) return false;

		if (fill_with_char(*handle, start_pos, length, ' ') == 0) return false;
		if (fill_with_attribute(*handle, start_pos, length, attribute) == 0) return false;
		return true;
	}

	bool clear_after_cursor(HANDLE handle, COORD cursor_pos, SIZE buffer_size, uint16_t attribute)
	{
		auto x = cursor_pos.X;
		auto y = cursor_pos.Y;
		if (x > buffer_size.cx)
		{
			y += 1;
			x = 0;
		}
		COORD start_pos{ x, y };
		uint32_t length = static_cast<uint32_t>(buffer_size.cx * buffer_size.cy);
		return clear(start_pos, length, attribute);
	}

	bool clear_before_cursor(HANDLE handle, COORD cursor_pos, SIZE buffer_size, uint16_t attribute)
	{
		COORD start_pos{ 0, 0 };
		uint32_t length = static_cast<uint32_t>(buffer_size.cx * cursor_pos.Y + cursor_pos.X + 1);
		return clear(start_pos, length, attribute);
	}

	bool clear_entire_screen(HANDLE handle, SIZE buffer_size, uint16_t attribute, bool clear_history = false)
	{
		COORD start_pos{ 0, 0 };
		uint32_t length = static_cast<uint32_t>(buffer_size.cx * buffer_size.cy);
		bool result = clear(start_pos, length, attribute);
		result &= set_cursor_pos({ 0, 0 });
		if (!clear_history) return result;
		
		CONSOLE_HISTORY_INFO chi{};
		chi.cbSize = sizeof(CONSOLE_HISTORY_INFO);
		if (!GetConsoleHistoryInfo(&chi)) return false;

		auto old_history_buffer_size = chi.HistoryBufferSize;
		chi.HistoryBufferSize = 0;
		if (!SetConsoleHistoryInfo(&chi)) return false;
		chi.HistoryBufferSize = old_history_buffer_size;
		if (!SetConsoleHistoryInfo(&chi)) return false;
		return result;
	}

	bool clear_line_before_cursor(HANDLE handle, COORD cursor_pos, SIZE buffer_size, uint16_t attribute)
	{
		COORD start_pos{ 0, cursor_pos.Y };
		uint32_t length = static_cast<uint32_t>(cursor_pos.X + 1);
		bool result = clear(start_pos, length, attribute);
		result &= set_cursor_pos({ static_cast<uint16_t>(cursor_pos.X), static_cast<uint16_t>(cursor_pos.Y) });
		return result;
	}

	bool clear_current_line(HANDLE handle, COORD cursor_pos, SIZE buffer_size, uint16_t attribute)
	{
		COORD start_pos{ 0, cursor_pos.Y };
		uint32_t length = static_cast<uint32_t>(buffer_size.cx);
		bool result = clear(start_pos, length, attribute);
		result &= set_cursor_pos({ 0, static_cast<uint16_t>(cursor_pos.Y) });
		return result;
	}

	bool clear_until_line(HANDLE handle, COORD cursor_pos, SIZE buffer_size, uint16_t attribute)
	{
		COORD start_pos{ cursor_pos.X, cursor_pos.Y };
		uint32_t length = static_cast<uint32_t>(buffer_size.cx - cursor_pos.X);
		bool result = clear(start_pos, length, attribute);
		result &= set_cursor_pos({ static_cast<uint16_t>(cursor_pos.X), static_cast<uint16_t>(cursor_pos.Y) });
		return result;
	}

	bool set_raw_mode(bool value)
	{
		auto handle = get_current_input_handle();
		if (!handle) return false;
		DWORD mode;
		if (!GetConsoleMode(*handle, &mode)) return false;
		if (value)
		{
			mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
		}
		else
		{
			mode |= (ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
		}
		return SetConsoleMode(*handle, mode) != 0;
	}

	bool is_raw_mode_enabled()
	{
		auto handle = get_current_input_handle();
		if (!handle) return false;
		DWORD mode;
		if (!GetConsoleMode(*handle, &mode)) return false;
		return (mode & (ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT)) == 0;
	}

	bool clear(clear_type type)
	{
		auto handle = get_current_output_handle();
		if (!handle) return false;

		CONSOLE_SCREEN_BUFFER_INFO csbi{};

		if (!GetConsoleScreenBufferInfo(*handle, &csbi)) return false;

		auto cursor_pos = csbi.dwCursorPosition;
		SIZE buffer_size{ static_cast<LONG>(csbi.dwSize.X), static_cast<LONG>(csbi.dwSize.Y) };
		auto current_attribute = csbi.wAttributes;

		switch (type)
		{
			case clear_type::all: return clear_entire_screen(*handle, buffer_size, current_attribute);
			case clear_type::all_and_history: return clear_entire_screen(*handle, buffer_size, current_attribute, true);
			case clear_type::from_cursor_down: return clear_after_cursor(*handle, cursor_pos, buffer_size, current_attribute);
			case clear_type::from_cursor_up: return clear_before_cursor(*handle, cursor_pos, buffer_size, current_attribute);
			case clear_type::from_cursor_to_start: return clear_line_before_cursor(*handle, cursor_pos, buffer_size, current_attribute);
			case clear_type::current_line: return clear_current_line(*handle, cursor_pos, buffer_size, current_attribute);
			case clear_type::until_newline: return clear_until_line(*handle, cursor_pos, buffer_size, current_attribute);
			default: return clear_entire_screen(*handle, buffer_size, current_attribute);
		}
	}

	std::optional<event> read_event()
	{
		auto handle = get_current_input_handle();
		if (!handle) return std::nullopt;

		INPUT_RECORD record{};
		DWORD events_read = 0;

		if (!ReadConsoleInput(*handle, &record, 1, &events_read)) return std::nullopt;
		if (events_read == 0) return std::nullopt;

		switch (record.EventType)
		{
			case MOUSE_EVENT:
			{
				mouse_event mevent{};
				mevent.pos = { static_cast<uint16_t>(record.Event.MouseEvent.dwMousePosition.X), static_cast<uint16_t>(record.Event.MouseEvent.dwMousePosition.Y) };
				switch (record.Event.MouseEvent.dwEventFlags)
				{
					case 0:
					{
						if (record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
						{
							mevent.event = mouse_button_event{ mouse_button::left, mouse_button_state::down };
							return event{ mevent };

						}
						else if (record.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
						{
							mevent.event = mouse_button_event{ mouse_button::right, mouse_button_state::down };
							return event{ mevent };
						}
						else if (record.Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)
						{
							mevent.event = mouse_button_event{ mouse_button::middle, mouse_button_state::down };
							return event{ mevent };
						}
						else
						{
							mevent.event = mouse_button_event{ mouse_button::left, mouse_button_state::up };
							return event{ mevent };
						}
					}
					break;
					case MOUSE_WHEELED:
					{
						int delta = static_cast<int>(static_cast<int16_t>(record.Event.MouseEvent.dwButtonState >> 16));
						if (delta == 0) return std::nullopt;
						if (delta > 0)
						{
							mevent.event = mouse_wheel_event{ mouse_wheel_direction::up, delta };
						}
						else
						{
							mevent.event = mouse_wheel_event{ mouse_wheel_direction::down, delta };
						}
						return event{ mevent };
					}
					break;
					case MOUSE_HWHEELED:
					{
						int delta = static_cast<int>(static_cast<int16_t>(record.Event.MouseEvent.dwButtonState >> 16));
						if (delta == 0) return std::nullopt;
						if (delta > 0)
						{
							mevent.event = mouse_wheel_event{ mouse_wheel_direction::right, delta };
						}
						else
						{
							mevent.event = mouse_wheel_event{ mouse_wheel_direction::left, delta };
						}
						return event{ mevent };
					}
					break;
					case MOUSE_MOVED:
					{
						mevent.event = mouse_move_event{ { static_cast<uint16_t>(record.Event.MouseEvent.dwMousePosition.X), static_cast<uint16_t>(record.Event.MouseEvent.dwMousePosition.Y) } };
						return event{ mevent };
					}
					break;
					default: return std::nullopt;
				}
			}
			case FOCUS_EVENT:
			{
				focus_changed_event fevent{ record.Event.FocusEvent.bSetFocus ? focus_change_type::gained : focus_change_type::lost };
				return event{ fevent };
			}
			case WINDOW_BUFFER_SIZE_EVENT:
			{
				auto width = static_cast<uint16_t>(record.Event.WindowBufferSizeEvent.dwSize.X);
				auto height = static_cast<uint16_t>(record.Event.WindowBufferSizeEvent.dwSize.Y);
				resize_event revent{ { width, height } };
				return event{ revent };
			}
			break;
			default: return std::nullopt;
		}
	}
}