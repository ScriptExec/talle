#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include <tuple>
#include <talle/core/backend.hpp>
#include <talle/cmd/commands.hpp>

namespace talle
{
	template<meta::backend backend_type>
	struct terminal
	{
	public:
		using writer_type = typename backend_type::writer_type;
		terminal(backend_type& backend) : backend_{ backend } {}
		~terminal()
		{
			backend_.flush();
		}

	private:
		backend_type& backend_;

	public:
		terminal& set_size(size new_size)
		{
			backend_.template execute<cmd::set_size>(new_size);
			return *this;
		}

		terminal& set_cursor_pos(position pos)
		{
			backend_.template execute<cmd::set_cursor_pos>(pos);
			return *this;
		}

		terminal& reset_cursor_pos()
		{
			backend_.template execute<cmd::set_cursor_pos>();
			return *this;
		}

		terminal& save_cursor_pos()
		{
			backend_.template execute<cmd::save_cursor_pos>();
			return *this;
		}

		terminal& restore_cursor_pos()
		{
			backend_.template execute<cmd::restore_cursor_pos>();
			return *this;
		}

		terminal& set_cursor_visible(bool value)
		{
			backend_.template execute<cmd::set_cursor_visible>(value);
			return *this;
		}

		terminal& set_cursor_style(cursor_style style)
		{
			backend_.template execute<cmd::set_cursor_style>(style);
			return *this;
		}

		terminal& reset_cursor_style()
		{
			return set_cursor_style(cursor_style::user_default);
		}

		terminal& set_title(const std::string& value)
		{
			backend_.template execute<cmd::set_title>(value);
			return *this;
		}

		terminal& enable_cursor_blinking(bool value)
		{
			backend_.template execute<cmd::toggle_cursor_blinking>(value);
			return *this;
		}

		terminal& enable_alternative_buffer(bool value)
		{
			backend_.template execute<cmd::toggle_alternative_buffer>(value);
			return *this;
		}

		terminal& enable_mouse_capture(bool value)
		{
			backend_.template execute<cmd::toggle_mouse_capture>(value);
			return *this;
		}

		terminal& clear(clear_type type = clear_type::all)
		{
			backend_.template execute<cmd::clear>(type);
			return *this;
		}

		terminal& set_foreground_color(const style::color& color)
		{
			backend_.template execute<cmd::set_foreground_color>(color);
			return *this;
		}

		terminal& reset_foreground_color()
		{
			backend_.template execute<cmd::set_foreground_color>(style::color::reset());
			return *this;
		}

		terminal& set_background_color(const style::color& color)
		{
			backend_.template execute<cmd::set_background_color>(color);
			return *this;
		}

		terminal& reset_background_color()
		{
			backend_.template execute<cmd::set_background_color>(style::color::reset());
			return *this;
		}

		terminal& set_underline_color(const style::color& color)
		{
			backend_.template execute<cmd::set_underline_color>(color);
			return *this;
		}

		terminal& reset_underline_color()
		{
			backend_.template execute<cmd::set_underline_color>(style::color::reset());
			return *this;
		}

		terminal& set_color(const style::color_part& color)
		{
			backend_.template execute<cmd::set_color>(color);
			return *this;
		}

		terminal& set_attribute(style::attribute attribute)
		{
			backend_.template execute<cmd::set_attribute>(attribute);
			return *this;
		}

		terminal& reset_attribute()
		{
			backend_.template execute<cmd::set_attribute>(style::attribute::reset);
			return *this;
		}

		template<typename... arguments> requires meta::writeable<writer_type, arguments...>
		terminal& write(arguments&&... args)
		{
			backend_.write(std::forward<arguments>(args)...);
			return *this;
		}

		template<typename... arguments> requires meta::writeable<writer_type, arguments...>
		terminal& writeln(arguments&&... args)
		{
			backend_.writeln(std::forward<arguments>(args)...);
			return *this;
		}

		terminal& newline(size_t count = 1)
		{
			for (size_t i = 0; i < count; ++i)
			{
				writeln();
			}
			return *this;
		}

		terminal& hyperlink(const style::hyperlink& link)
		{
			backend_.template execute<cmd::write_hyperlink>(link);
			return *this;
		}

		terminal& flush()
		{
			backend_.flush();
			return *this;
		}

		std::optional<event> read_event()
		{
			return sys::read_event();
		}

		std::optional<size> size()
		{
			return backend_.get_size();
		}

		template<typename writeable_type> requires meta::writeable<writer_type, writeable_type>
		terminal& operator<<(const writeable_type& value)
		{
			return write(value);
		}
	};
}
