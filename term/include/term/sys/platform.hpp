#pragma once
#include <cstdint>
#include <optional>
#include <term/utils/position.hpp>
#include <term/core/clear_type.hpp>
#include <term/event/event.hpp>

namespace term::sys
{
	bool setup();
	bool set_cursor_visible(bool value);
	bool set_cursor_pos(position pos);
	bool toggle_alternative_buffer(bool value);
	bool toggle_mouse_capture(bool value);
	bool set_raw_mode(bool value);
	bool is_raw_mode_enabled();
	std::optional<position> get_cursor_pos();
	bool clear(clear_type type);

	std::optional<event> read_event();

}
