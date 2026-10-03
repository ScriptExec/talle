#include <iostream>
#include <term/core/terminal.hpp>
#include <term/core/backend.hpp>

int main(int argc, char* argv[])
{
	using namespace term;

	auto backend = term::backend{ std::cout };
	auto term = term::terminal{ backend };
	backend.set_raw_mode(true);
	term.set_cursor_visible(false);
	term.enable_mouse_capture(true);
	/*
	term.write("Hello World!");
	auto cpos = backend.get_cursor_pos().value();
	term.set_cursor_visible(false)
		.enable_alternative_buffer(true);
	term.reset_cursor_pos()
		.write("Hella World!")
		.enable_alternative_buffer(false)
		.reset_cursor_pos()
		.set_cursor_visible(true)
		.clear()
		.write("Last Cursor Position: (", cpos.x, ", ", cpos.y, ")\n");
	*/

	while (true)
	{
		auto cpos = backend.get_cursor_pos().value();
		auto event = term.read_event();
		if (!event.has_value()) continue;

		std::string message;
		if (auto event_mouse = event->try_get<mouse_event>())
		{
			if (auto event_mouse_move = event_mouse->try_get<mouse_move_event>())
			{
				message = "Mouse Move: (" + std::to_string(event_mouse_move->pos.x) + ", " + std::to_string(event_mouse_move->pos.y) + ")";
			}
			else if (auto event_mouse_button = event_mouse->try_get<mouse_button_event>())
			{
				message = "Mouse Button (" + std::string(event_mouse_button->state == mouse_button_state::down ? "Down" : "Up") + "): ";
				switch (event_mouse_button->button)
				{
					case mouse_button::left: message += "Left"; break;
					case mouse_button::middle: message += "Middle"; break;
					case mouse_button::right: message += "Right"; break;
				}
			}
			else if (auto event_mouse_wheel = event_mouse->try_get<mouse_wheel_event>())
			{
				std::string direction;
				switch (event_mouse_wheel->direction)
				{
					case mouse_wheel_direction::up: direction = "Up"; break;
					case mouse_wheel_direction::down: direction = "Down"; break;
					case mouse_wheel_direction::left: direction = "Left"; break;
					case mouse_wheel_direction::right: direction = "Right"; break;
				}
				message = "Mouse Wheel (" + direction + "): " + std::to_string(event_mouse_wheel->delta);
			}
		}
		else if (auto event_focus = event->try_get<focus_changed_event>())
		{
			if (event_focus->type == focus_change_type::gained)
			{
				message = "Focus Gained";
			}
			else if (event_focus->type == focus_change_type::lost)
			{
				message = "Focus Lost";
			}
		}
		else if (auto event_resize = event->try_get<resize_event>())
		{
			message = "Terminal Resized: (" + std::to_string(event_resize->size.width) + ", " + std::to_string(event_resize->size.height) + ")";
		}

		if (!message.empty())
		{
			auto cpos = backend.get_cursor_pos().value();
			term.reset_cursor_pos()
				.write(message);
			auto chars_left = (int)cpos.x - (int)message.size();
			if (chars_left > 0)
			{
				term.write(std::string(chars_left, ' '));
			}
		}
	}
	term.enable_mouse_capture(false);
	term.set_cursor_visible(true);
	backend.set_raw_mode(false);
	return 0;
}