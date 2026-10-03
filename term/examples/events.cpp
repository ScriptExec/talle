#include <iostream>
#include <string>
#include <term/core/terminal.hpp>
#include <term/core/backend.hpp>

std::string_view map_key_modifier_name(term::key_modifier modifier)
{
	switch (modifier)
	{
		case term::key_modifier::shift: return "Shift";
		case term::key_modifier::ctrl: return "Ctrl";
		case term::key_modifier::alt: return "Alt";
		case term::key_modifier::super: return "Super";
		default: return "";
	}
}

void add_key_modifiers(std::string& message, term::key_modifiers mods)
{
	if (message.empty()) return;

	message += " [";
	size_t comma_budget = mods.modifier_count();
	if (comma_budget > 0)
	{
		--comma_budget;
		if (mods.has(term::key_modifier::ctrl))
		{
			message += map_key_modifier_name(term::key_modifier::ctrl);
			if (comma_budget-- > 0) message += ", ";
		}
		if (mods.has(term::key_modifier::alt))
		{
			message += map_key_modifier_name(term::key_modifier::alt);
			if (comma_budget-- > 0) message += ", ";
		}
		if (mods.has(term::key_modifier::shift))
		{
			message += map_key_modifier_name(term::key_modifier::shift);
			if (comma_budget-- > 0) message += ", ";
		}
		if (mods.has(term::key_modifier::super))
		{
			message += map_key_modifier_name(term::key_modifier::super);
			if (comma_budget-- > 0) message += ", ";
		}
	}
	message += "]";

}

int main(int argc, char* argv[])
{
	using namespace term;

	auto backend = term::backend{ std::cout };
	auto term = term::terminal{ backend };
	backend.set_raw_mode(true);
	term.enable_mouse_capture(true);
	term.set_cursor_visible(false);
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
		std::string message;
		if (event.has_value())
		{
			if (auto event_key = event->try_get<key_event>())
			{
				message = "Key (" + std::string(event_key->type == key_event_type::press ? "Press" : (event_key->type == key_event_type::repeat ? "Repeat" : "Release")) + "): ";
				if (event_key->key.is<key::code>())
				{
					message += std::to_string(static_cast<int>(event_key->key.get<key::code>()));
				}
				else if (event_key->key.is<key::fn>())
				{
					message += "F" + std::to_string(event_key->key.get<key::fn>().number);
				}
				else if (event_key->key.is<key::chr>())
				{
					message += "'" + std::string(1, static_cast<char>(event_key->key.get<key::chr>().c)) + "'";
				}
				add_key_modifiers(message, event_key->modifiers);
			}
			else if (auto event_mouse = event->try_get<mouse_event>())
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
				message += " at (" + std::to_string(event_mouse->pos.x) + ", " + std::to_string(event_mouse->pos.y) + ")";
				add_key_modifiers(message, event_mouse->modifiers);
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
		}

		if (!message.empty())
		{
			term.writeln(message);
			/*
			auto cpos = backend.get_cursor_pos().value();
			term.reset_cursor_pos()
				.write(message);
			auto chars_left = (int)cpos.x - (int)message.size();
			if (chars_left > 0)
			{
				term.write(std::string(chars_left, ' '));
			}
			*/
		}
	}
	term.set_cursor_visible(true);
	term.enable_mouse_capture(false);
	backend.set_raw_mode(false);
	return 0;
}