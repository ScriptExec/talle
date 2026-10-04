#pragma once
#include <cstdint>
#include <optional>
#include <string>

#define VC_EXTRALEAN
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <talle/utils/position.hpp>
#include <talle/event/event.hpp>
#include <talle/event/resize_event.hpp>
#include <talle/input/key.hpp>
#include <talle/event/key_event.hpp>
#include <codecvt>

namespace talle::sys
{
	std::optional<handle> win_current_output_handle()
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

	std::optional<handle> win_current_input_handle()
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

	bool setup()
	{
		auto handle = win_current_output_handle();
		if (!handle) return false;
		DWORD mode;
		if (!SetConsoleOutputCP(CP_UTF8)) return false;
		if (!GetConsoleMode(*handle, &mode)) return false;
		mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		return SetConsoleMode(*handle, mode);
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

	bool set_title(const std::string& title)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wide_title = converter.from_bytes(title);
		return SetConsoleTitleW(wide_title.c_str());
	}

	bool toggle_alternative_buffer(bool value)
	{
		auto handle = win_current_output_handle();
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
			mode |= ENABLE_EXTENDED_FLAGS;
			mode &= ~ENABLE_QUICK_EDIT_MODE;
			mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
		}
		else
		{
			mode |= ENABLE_EXTENDED_FLAGS;
			mode |= ENABLE_QUICK_EDIT_MODE;
			mode |= (ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
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
}