#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <atomic>
#include <limits>
#include <codecvt>
#include <csignal>
#include <limits>

#define VC_EXTRALEAN
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <talle/utils/position.hpp>
#include <talle/event/event.hpp>
#include <talle/event/resize_event.hpp>
#include <talle/input/key.hpp>
#include <talle/event/key_event.hpp>
#include <talle/style/color.hpp>
#include <talle/style/color_part.hpp>

namespace talle::sys
{
	struct platform_data
	{
		uint32_t original_console_in_mode{};
		uint32_t original_console_out_mode{};
		std::atomic<uint32_t> original_console_color{ std::numeric_limits<uint32_t>::max() };
		std::atomic<uint32_t> saved_cursor_pos{ 0 };
		HANDLE last_buffer_handle{ INVALID_HANDLE_VALUE };
	} data;

	std::optional<handle> stdout_handle()
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (handle == INVALID_HANDLE_VALUE) return std::nullopt;
		return handle;
	}

	std::optional<handle> stderr_handle()
	{
		HANDLE handle = GetStdHandle(STD_ERROR_HANDLE);
		if (handle == INVALID_HANDLE_VALUE) return std::nullopt;
		return handle;
	}

	std::optional<handle> stdin_handle()
	{
		HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
		if (handle == INVALID_HANDLE_VALUE) return std::nullopt;
		return handle;
	}

	std::optional<HANDLE> win_current_output_handle()
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

	std::optional<HANDLE> win_current_input_handle()
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

	std::optional<handle> current_output_handle()
	{
		return win_current_output_handle();
	}

	std::optional<handle> current_input_handle()
	{
		return win_current_input_handle();
	}

	void setup_data()
	{
		auto out_handle = win_current_output_handle();
		if (out_handle)
		{
			DWORD mode;
			if (GetConsoleMode(*out_handle, &mode))
			{
				data.original_console_out_mode = mode;
			}
		}
		auto in_handle = win_current_input_handle();
		if (in_handle)
		{
			DWORD mode;
			if (GetConsoleMode(*in_handle, &mode))
			{
				data.original_console_in_mode = mode;
			}
		}
	}

	void restore_data()
	{
		auto out_handle = win_current_output_handle();
		if (out_handle.has_value())
		{
			SetConsoleMode(*out_handle, data.original_console_out_mode);
		}
		auto in_handle = win_current_input_handle();
		if (in_handle.has_value())
		{
			SetConsoleMode(*in_handle, data.original_console_in_mode);
		}
	}

	void signal_handler(int signal)
	{
		if (signal == SIGINT or signal == SIGTERM or signal == SIGABRT)
		{
			restore_data();
			std::exit(0);
		}
	}

	bool setup()
	{
		setup_data();

		auto handle = win_current_output_handle();
		if (!handle) return false;
		DWORD mode;
		if (!SetConsoleOutputCP(CP_UTF8)) return false;
		if (!GetConsoleMode(*handle, &mode)) return false;
		std::signal(SIGINT, signal_handler);
		std::signal(SIGABRT, signal_handler);
		std::signal(SIGTERM, signal_handler);
		mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		return SetConsoleMode(*handle, mode);
	}

	bool set_size(size new_size)
	{
		//terminal size too small
		if (new_size.width <= 1 or new_size.height <= 1) return false;

		auto handle = win_current_output_handle();
		if (!handle) return false;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(*handle, &csbi)) return false;

		bool resize_buffer = false;

		auto width = static_cast<int16_t>(new_size.width);
		auto height = static_cast<int16_t>(new_size.height);

		auto window_rect = csbi.srWindow;
		auto buffer_size = csbi.dwSize;

		COORD new_size_coord = buffer_size;


		if (buffer_size.X < width or window_rect.Left + width)
		{
			if (window_rect.Left >= std::numeric_limits<int16_t>::max() - width)
			{
				//terminal width is too large
				return false;
			}
			new_size_coord.X = window_rect.Left + width;
			resize_buffer = true;
		}

		if (buffer_size.Y < window_rect.Top + height)
		{
			if (window_rect.Top >= std::numeric_limits<int16_t>::max() - height)
			{
				//terminal height is too large
				return false;
			}
			new_size_coord.Y = window_rect.Top + height;
			resize_buffer = true;
		}

		if (resize_buffer)
		{
			COORD coord{};
			coord.X = new_size_coord.X - 1;
			coord.Y = new_size_coord.Y - 1;
			if (!SetConsoleScreenBufferSize(*handle, coord)) return false;
		}

		//resizes and preserves the current window position
		window_rect.Bottom = window_rect.Top + height - 1;
		window_rect.Right = window_rect.Left + width - 1;
		if (!SetConsoleWindowInfo(*handle, TRUE, &window_rect)) return false;

		if (resize_buffer)
		{
			COORD coord{};
			coord.X = new_size_coord.X - 1;
			coord.Y = new_size_coord.Y - 1;
			if (!SetConsoleScreenBufferSize(*handle, coord)) return false;
		}

		COORD bounds = GetLargestConsoleWindowSize(*handle);
		if (bounds.X == 0 or bounds.Y == 0) return false;

		if (width > bounds.X or height > bounds.Y)
		{
			//terminal width or height is too large
			return false;
		}
		return true;
	}

	std::optional<size> get_size()
	{
		auto handle = win_current_output_handle();
		if (!handle) return std::nullopt;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(*handle, &csbi)) return std::nullopt;
		uint16_t width = static_cast<uint16_t>(csbi.srWindow.Right - csbi.srWindow.Left + 1);
		uint16_t height = static_cast<uint16_t>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
		return size{ width, height };
	}

	bool set_cursor_visible(bool value)
	{
		auto handle = win_current_output_handle();
		if (!handle) return false;

		CONSOLE_CURSOR_INFO cci;
		GetConsoleCursorInfo(*handle, &cci);
		cci.bVisible = value;
		SetConsoleCursorInfo(*handle, &cci);
		return true;
	}

	bool set_cursor_pos(position pos)
	{
		auto handle = win_current_output_handle();
		if (!handle) return false;
		COORD coord;
		coord.X = pos.x;
		coord.Y = pos.y;
		return SetConsoleCursorPosition(*handle, coord);
	}

	bool save_cursor_pos()
	{
		auto pos = get_cursor_pos();
		if (!pos.has_value()) return false;
		data.saved_cursor_pos.store((pos->x << 16) | pos->y, std::memory_order_relaxed);
		return true;
	}

	bool restore_cursor_pos()
	{
		auto pos = static_cast<uint32_t>(data.saved_cursor_pos.load(std::memory_order_relaxed));
		return set_cursor_pos({ static_cast<uint16_t>(pos >> 16), static_cast<uint16_t>(pos & 0xFFFF) });
	}

	bool set_title(const std::string& title)
	{
		std::wstring wide_title;
		if (MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, nullptr, 0) > 0)
		{
			wide_title.resize(MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, nullptr, 0));
			MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, wide_title.data(), static_cast<int>(wide_title.size()));
		}
		else
		{
			return false;
		}
		return SetConsoleTitleW(wide_title.c_str());
	}

	bool toggle_alternative_buffer(bool value)
	{
		auto handle = win_current_output_handle();
		if (!handle) return false;

		HANDLE new_handle;
		if (value)
		{
			data.last_buffer_handle = *handle;
			SECURITY_ATTRIBUTES sa;
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.lpSecurityDescriptor = nullptr;
			sa.bInheritHandle = TRUE;
			new_handle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, CONSOLE_TEXTMODE_BUFFER, nullptr);
			if (new_handle == INVALID_HANDLE_VALUE) return false;
		}
		else
		{
			if (!CloseHandle(*handle)) return false;
			new_handle = data.last_buffer_handle;
			if (new_handle == INVALID_HANDLE_VALUE) return false;
		}
		return SetConsoleActiveScreenBuffer(new_handle);
	}

	bool toggle_mouse_capture(bool value)
	{
		auto handle = win_current_input_handle();
		if (!handle) return false;
		DWORD mode;
		if (!GetConsoleMode(*handle, &mode)) return false;
		if (value)
		{
			mode |= ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT;
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
		auto handle = win_current_output_handle();
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
		auto handle = win_current_output_handle();
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
		auto handle = win_current_input_handle();
		if (!handle) return false;
		DWORD mode;
		if (!GetConsoleMode(*handle, &mode)) return false;
		if (value)
		{
			data.original_console_in_mode = mode;
			mode |= ENABLE_EXTENDED_FLAGS;
			mode &= ~ENABLE_QUICK_EDIT_MODE;
			mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
		}
		else
		{
			/*
			mode |= ENABLE_EXTENDED_FLAGS;
			mode |= ENABLE_QUICK_EDIT_MODE;
			mode |= (ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
			*/
			mode = data.original_console_in_mode;
		}
		return SetConsoleMode(*handle, mode) != 0;
	}

	bool is_raw_mode_enabled()
	{
		auto handle = win_current_input_handle();
		if (!handle) return false;
		DWORD mode;
		if (!GetConsoleMode(*handle, &mode)) return false;
		return (mode & (ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT)) == 0;
	}

	bool clear(clear_type type)
	{
		auto handle = win_current_output_handle();
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

	key_modifiers to_key_modifiers(DWORD state)
	{
		key_modifiers modifiers;
		if (state & SHIFT_PRESSED)
		{
			modifiers.set(key_modifier::shift, true);
		}
		if (state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
		{
			modifiers.set(key_modifier::ctrl, true);
		}
		if (state & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
		{
			modifiers.set(key_modifier::alt, true);
		}
		return modifiers;
	}

	wchar_t raw_key_char(const KEY_EVENT_RECORD& e)
	{
		UINT ch = MapVirtualKey(e.wVirtualKeyCode, MAPVK_VK_TO_CHAR);
		return static_cast<wchar_t>(ch & 0x7FFF);
	}

	std::optional<key> to_key(const KEY_EVENT_RECORD& e)
	{
		switch (e.wVirtualKeyCode)
		{
			case VK_BACK: return key{ key::code::backspace };
			case VK_RETURN: return key{ key::code::enter };
			case VK_LEFT: return key{ key::code::left };
			case VK_RIGHT: return key{ key::code::right };
			case VK_UP: return key{ key::code::up };
			case VK_DOWN: return key{ key::code::down };
			case VK_HOME: return key{ key::code::home };
			case VK_END: return key{ key::code::end };
			case VK_PRIOR: return key{ key::code::pageup };
			case VK_NEXT: return key{ key::code::pagedown };
			case VK_TAB: return key{ key::code::tab };
			case VK_DELETE: return key{ key::code::delete_ };
			case VK_INSERT: return key{ key::code::insert };
			case VK_ESCAPE: return key{ key::code::escape };
			case VK_F1: [[fallthrough]];
			case VK_F2: [[fallthrough]];
			case VK_F3: [[fallthrough]];
			case VK_F4: [[fallthrough]];
			case VK_F5: [[fallthrough]];
			case VK_F6: [[fallthrough]];
			case VK_F7: [[fallthrough]];
			case VK_F8: [[fallthrough]];
			case VK_F9: [[fallthrough]];
			case VK_F10: [[fallthrough]];
			case VK_F11: [[fallthrough]];
			case VK_F12: return key{ key::fn{ static_cast<uint8_t>(e.wVirtualKeyCode - VK_F1 + 1) } };
			//TODO: Should F13-F24 be handled?
			default: break;
		}

		//printable characters
		wchar_t c = e.uChar.UnicodeChar;
		if ((e.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) and e.wVirtualKeyCode >= 'A' and e.wVirtualKeyCode <= 'Z')
		{
			c = static_cast<wchar_t>(e.wVirtualKeyCode + ('a' - 'A'));
		}

		if (c != L'\0')
		{
			//wchar_t c = e.uChar.UnicodeChar;
			/*
			auto c = raw_key_char(e);
			if (c <= 0xFF)
			{
			}
			*/
			return key{ key::chr{ static_cast<uint8_t>(c) } };
		}
		return std::nullopt;
	}

	std::optional<event> read_event()
	{
		auto handle = win_current_input_handle();
		if (!handle) return std::nullopt;

		INPUT_RECORD record{};
		DWORD events_read{};

		DWORD events_waiting{};
		if (GetNumberOfConsoleInputEvents(*handle, &events_waiting) and events_waiting == 0) return std::nullopt;
		if (!ReadConsoleInput(*handle, &record, 1, &events_read)) return std::nullopt;
		if (events_read == 0) return std::nullopt;

		switch (record.EventType)
		{
			case KEY_EVENT:
			{
				auto key = to_key(record.Event.KeyEvent);
				key_event kev{ key.value_or(key::code::unknown) };
				kev.type = !record.Event.KeyEvent.bKeyDown ? key_event_type::release : (record.Event.KeyEvent.wRepeatCount > 1 ? key_event_type::repeat : key_event_type::press);
				kev.modifiers = to_key_modifiers(record.Event.KeyEvent.dwControlKeyState);
				return event{ kev };
			}
			break;
			case MOUSE_EVENT:
			{
				mouse_event mev{};
				mev.pos = { static_cast<uint16_t>(record.Event.MouseEvent.dwMousePosition.X), static_cast<uint16_t>(record.Event.MouseEvent.dwMousePosition.Y) };
				mev.modifiers = to_key_modifiers(record.Event.MouseEvent.dwControlKeyState);

				switch (record.Event.MouseEvent.dwEventFlags)
				{
					case 0:
					{
						if (record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
						{
							mev.event = mouse_button_event{ mouse_button::left, mouse_button_state::down };
							return event{ mev };

						}
						else if (record.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
						{
							mev.event = mouse_button_event{ mouse_button::right, mouse_button_state::down };
							return event{ mev };
						}
						else if (record.Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)
						{
							mev.event = mouse_button_event{ mouse_button::middle, mouse_button_state::down };
							return event{ mev };
						}
						else
						{
							mev.event = mouse_button_event{ mouse_button::left, mouse_button_state::up };
							return event{ mev };
						}
					}
					break;
					case MOUSE_WHEELED:
					{
						int delta = static_cast<int>(static_cast<int16_t>(record.Event.MouseEvent.dwButtonState >> 16));
						if (delta == 0) return std::nullopt;
						if (delta > 0)
						{
							mev.event = mouse_wheel_event{ mouse_wheel_direction::up, delta };
						}
						else
						{
							mev.event = mouse_wheel_event{ mouse_wheel_direction::down, delta };
						}
						return event{ mev };
					}
					break;
					case MOUSE_HWHEELED:
					{
						int delta = static_cast<int>(static_cast<int16_t>(record.Event.MouseEvent.dwButtonState >> 16));
						if (delta == 0) return std::nullopt;
						if (delta > 0)
						{
							mev.event = mouse_wheel_event{ mouse_wheel_direction::right, delta };
						}
						else
						{
							mev.event = mouse_wheel_event{ mouse_wheel_direction::left, delta };
						}
						return event{ mev };
					}
					break;
					case MOUSE_MOVED:
					{
						mev.event = mouse_move_event{ { static_cast<uint16_t>(record.Event.MouseEvent.dwMousePosition.X), static_cast<uint16_t>(record.Event.MouseEvent.dwMousePosition.Y) } };
						return event{ mev };
					}
					break;
					default: return std::nullopt;
				}
			}
			case FOCUS_EVENT:
			{
				focus_changed_event fev{ record.Event.FocusEvent.bSetFocus ? focus_change_type::gained : focus_change_type::lost };
				return event{ fev };
			}
			case WINDOW_BUFFER_SIZE_EVENT:
			{
				auto width = static_cast<uint16_t>(record.Event.WindowBufferSizeEvent.dwSize.X);
				auto height = static_cast<uint16_t>(record.Event.WindowBufferSizeEvent.dwSize.Y);
				resize_event rev{ { width, height } };
				return event{ rev };
			}
			break;
			default: return std::nullopt;
		}
	}

	bool init_color()
	{
		auto saved_mode = data.original_console_color.load(std::memory_order_relaxed);
		if (saved_mode == std::numeric_limits<uint32_t>::max())
		{
			auto handle = win_current_output_handle();
			if (!handle) return false;
			CONSOLE_SCREEN_BUFFER_INFO csbi{};
			if (!GetConsoleScreenBufferInfo(*handle, &csbi)) return false;
			data.original_console_color.store(csbi.wAttributes, std::memory_order_relaxed);
		}
		return true;
	}

	uint16_t get_original_console_color()
	{
		return static_cast<uint16_t>(data.original_console_color.load(std::memory_order_relaxed));
	}

	constexpr uint16_t foreground_mask = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	constexpr uint16_t background_mask = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;

	constexpr uint16_t foreground_mask_no_intens = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	constexpr uint16_t background_mask_no_intens = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;

	uint16_t map_fg_color(const style::color& color)
	{
		if (auto base_col = color.try_get<style::color::base>())
		{
			switch (*base_col)
			{
				case style::color::base::black: return 0;
				case style::color::base::dark_grey: return FOREGROUND_INTENSITY | 0;
				case style::color::base::red: return FOREGROUND_INTENSITY | FOREGROUND_RED;
				case style::color::base::dark_red: return FOREGROUND_RED;
				case style::color::base::green: return FOREGROUND_INTENSITY | FOREGROUND_GREEN;
				case style::color::base::dark_green: return FOREGROUND_GREEN;
				case style::color::base::yellow: return FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
				case style::color::base::dark_yellow: return FOREGROUND_RED | FOREGROUND_GREEN;
				case style::color::base::blue: return FOREGROUND_INTENSITY | FOREGROUND_BLUE;
				case style::color::base::dark_blue: return FOREGROUND_BLUE;
				case style::color::base::magenta: return FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE;
				case style::color::base::dark_magenta: return FOREGROUND_RED | FOREGROUND_BLUE;
				case style::color::base::cyan: return FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;
				case style::color::base::dark_cyan: return FOREGROUND_GREEN | FOREGROUND_BLUE;
				case style::color::base::white: return FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
				case style::color::base::grey: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
				case style::color::base::reset:
				{
					auto original_color = get_original_console_color();
					return static_cast<uint16_t>(original_color & (~background_mask));
				}
				break;
				default: return 0;
			}
		}
		//call_winapi handles rgb and ansi
		return 0;
	}

	uint16_t map_bg_color(const style::color& color)
	{
		if (auto base_col = color.try_get<style::color::base>())
		{
			switch (*base_col)
			{
				case style::color::base::black: return 0;
				case style::color::base::dark_grey: return BACKGROUND_INTENSITY | 0;
				case style::color::base::red: return BACKGROUND_INTENSITY | BACKGROUND_RED;
				case style::color::base::dark_red: return BACKGROUND_RED;
				case style::color::base::green: return BACKGROUND_INTENSITY | BACKGROUND_GREEN;
				case style::color::base::dark_green: return BACKGROUND_GREEN;
				case style::color::base::yellow: return BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN;
				case style::color::base::dark_yellow: return BACKGROUND_RED | BACKGROUND_GREEN;
				case style::color::base::blue: return BACKGROUND_INTENSITY | BACKGROUND_BLUE;
				case style::color::base::dark_blue: return BACKGROUND_BLUE;
				case style::color::base::magenta: return BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE;
				case style::color::base::dark_magenta: return BACKGROUND_RED | BACKGROUND_BLUE;
				case style::color::base::cyan: return BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE;
				case style::color::base::dark_cyan: return BACKGROUND_GREEN | BACKGROUND_BLUE;
				case style::color::base::white: return BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
				case style::color::base::grey: return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
				case style::color::base::reset:
				{
					auto original_color = get_original_console_color();
					return static_cast<uint16_t>(original_color & (~foreground_mask));
				}
				break;
				default: return 0;
			}
		}
		//call_winapi handles rgb and ansi
		return 0;
	}

	uint16_t map_ul_color(const style::color& color)
	{
		//not supported
		return 0;
	}

	uint16_t map_color_part_color(const style::color_part& part)
	{
		if (auto fg = part.try_get<style::color_part::foreground>())
		{
			return map_fg_color(fg->value);
		}
		else if (auto bg = part.try_get<style::color_part::background>())
		{
			return map_bg_color(bg->value);
		}
		else if (auto ul = part.try_get<style::color_part::underline>())
		{
			return map_ul_color(ul->value);
		}
		return 0;
	}

	bool set_foreground_color(const style::color& color)
	{
		init_color();

		auto color_u16 = map_fg_color(color);
		auto handle = win_current_output_handle();
		if (!handle) return false;
		CONSOLE_SCREEN_BUFFER_INFO csbi{};
		if (!GetConsoleScreenBufferInfo(*handle, &csbi)) return false;

		WORD color_bg = csbi.wAttributes & background_mask_no_intens;
		WORD color_new = color_u16 | color_bg;

		if ((csbi.wAttributes & FOREGROUND_INTENSITY) != 0)
		{
			color_new |= FOREGROUND_INTENSITY;
		}

		if (!SetConsoleTextAttribute(*handle, color_new)) return false;
		return true;
	}

	bool set_background_color(const style::color& color)
	{
		init_color();

		auto color_u16 = map_bg_color(color);
		auto handle = win_current_output_handle();
		if (!handle) return false;
		CONSOLE_SCREEN_BUFFER_INFO csbi{};
		if (!GetConsoleScreenBufferInfo(*handle, &csbi)) return false;

		WORD color_bg = csbi.wAttributes & foreground_mask_no_intens;
		WORD color_new = color_u16 | color_bg;

		if ((csbi.wAttributes & BACKGROUND_INTENSITY) != 0)
		{
			color_new |= BACKGROUND_INTENSITY;
		}

		if (!SetConsoleTextAttribute(*handle, color_new)) return false;
		return true;
	}

	bool set_underline_color(const style::color& color)
	{
		//not supported
		return false;
	}
}