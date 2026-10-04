#include <iostream>
#include <string>
#include <talle/core/terminal.hpp>
#include <talle/core/backend.hpp>

std::string_view map_key_modifier_name(talle::key_modifier modifier)
{
	switch (modifier)
	{
		case talle::key_modifier::shift: return "Shift";
		case talle::key_modifier::ctrl: return "Ctrl";
		case talle::key_modifier::alt: return "Alt";
		case talle::key_modifier::super: return "Super";
		default: return "";
	}
}

void add_key_modifiers(std::string& message, talle::key_modifiers mods)
{
	if (message.empty()) return;

	message += " [";
	size_t comma_budget = mods.modifier_count();
	if (comma_budget > 0)
	{
		--comma_budget;
		if (mods.has(talle::key_modifier::ctrl))
		{
			message += map_key_modifier_name(talle::key_modifier::ctrl);
			if (comma_budget-- > 0) message += ", ";
		}
		if (mods.has(talle::key_modifier::alt))
		{
			message += map_key_modifier_name(talle::key_modifier::alt);
			if (comma_budget-- > 0) message += ", ";
		}
		if (mods.has(talle::key_modifier::shift))
		{
			message += map_key_modifier_name(talle::key_modifier::shift);
			if (comma_budget-- > 0) message += ", ";
		}
		if (mods.has(talle::key_modifier::super))
		{
			message += map_key_modifier_name(talle::key_modifier::super);
			if (comma_budget-- > 0) message += ", ";
		}
	}
	message += "]";

}

int main(int argc, char* argv[])
{
	using namespace talle;

	auto back = backend{ std::cout };
	auto term = terminal{ back };
	back.set_raw_mode(true);
	term.enable_mouse_capture(true);
	term.set_cursor_visible(false);
	/*
	term.write("Hello World!");
	auto cpos = back.get_cursor_pos().value();
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
		auto cpos = back.get_cursor_pos().value();
		auto event = term.read_event();
		std::string message;
		if (event.has_value())
		{
			if (auto event_key = event->try_get<key_event>())
			{
				message = "Key (" + std::string(event_key->type == key_event_type::press ? "Press" : (event_key->type == key_event_type::repeat ? "Repeat" : "Release")) + "): ";
				if (event_key->key.is<key::code>())
				{
					auto c = event_key->key.get<key::code>();
					switch (c)
					{
						case key::code::backspace: message += "Backspace"; break;
						case key::code::enter: message += "Enter"; break;
						case key::code::left: message += "Left"; break;
						case key::code::right: message += "Right"; break;
						case key::code::up: message += "Up"; break;
						case key::code::down: message += "Down"; break;
						case key::code::home: message += "Home"; break;
						case key::code::end: message += "End"; break;
						case key::code::pageup: message += "PageUp"; break;
						case key::code::pagedown: message += "PageDown"; break;
						case key::code::tab: message += "Tab"; break;
						case key::code::backtab: message += "BackTab"; break;
						case key::code::delete_: message += "Delete"; break;
						case key::code::insert: message += "Insert"; break;
						case key::code::escape: message += "Escape"; break;
					}
				}
				else if (auto event_key_fn = event_key->key.try_get<key::fn>())
				{
					message += "F" + std::to_string(event_key_fn->number);
				}
				else if (auto event_key_chr = event_key->key.try_get<key::chr>())
				{
					if (event_key_chr->c == 'q' and event_key->modifiers.has(talle::key_modifier::ctrl))
					{
						return 0;
					}
					message += "'" + std::string(1, static_cast<char>(event_key_chr->c)) + "'";
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
			//term.writeln(message);
			
			auto cpos = back.get_cursor_pos().value();
			term.reset_cursor_pos()
				.write(message);
			auto chars_left = (int)cpos.x - (int)message.size();
			if (chars_left > 0)
			{
				term.write(std::string(chars_left, ' '));
			}
			
		}
	}
	term.set_cursor_visible(true);
	term.enable_mouse_capture(false);
	back.set_raw_mode(false);
	return 0;
}