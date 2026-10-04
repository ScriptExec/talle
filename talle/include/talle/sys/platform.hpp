#pragma once
#include <cstdint>
#include <optional>
#include <talle/utils/position.hpp>
#include <talle/core/clear_type.hpp>
#include <talle/event/event.hpp>
#include <string>

namespace talle::sys
{
	/**
	* @brief Platform specific handle type.
	* 
	* HANDLE on Windows, on Unix-like systems - file descriptor (int).
	*/
	using handle = void*;

	std::optional<handle> stdout_handle();
	std::optional<handle> stderr_handle();
	std::optional<handle> stdin_handle();

	std::optional<handle> current_output_handle();
	std::optional<handle> current_input_handle();

	bool setup();
	bool set_cursor_visible(bool value);
	bool set_cursor_pos(position pos);
	bool set_title(const std::string& title);
	bool toggle_alternative_buffer(bool value);
	bool toggle_mouse_capture(bool value);
	bool set_raw_mode(bool value);
	bool is_raw_mode_enabled();
	std::optional<position> get_cursor_pos();
	bool clear(clear_type type);

	std::optional<event> read_event();

}
