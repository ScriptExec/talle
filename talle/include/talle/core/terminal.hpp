#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include <tuple>
#include <talle/core/backend.hpp>

#include <talle/cmd/move_cursor.hpp>
#include <talle/cmd/toggle_alternative_buffer.hpp>
#include <talle/cmd/toggle_mouse_capture.hpp>
#include <talle/cmd/clear.hpp>
#include <talle/cmd/set_cursor_visible.hpp>

namespace talle
{
	template<meta::backend backend_type>
	struct terminal
	{
	public:
		using writer_type = typename backend_type::writer_type;
		terminal(backend_type& backend) : backend_{ backend } {}

	private:
		backend_type& backend_;

	public:
		terminal& set_cursor_pos(position pos)
		{
			backend_.template execute<cmd::move_cursor>(pos);
			return *this;
		}

		terminal& reset_cursor_pos()
		{
			backend_.template execute<cmd::move_cursor>();
			return *this;
		}

		terminal& set_cursor_visible(bool value)
		{
			backend_.template execute<cmd::set_cursor_visible>(value);
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

		std::optional<event> read_event()
		{
			return sys::read_event();
		}
		/*
		static bool init();
		static void update();
		static void close();
		static bool is_terminal();

		static void enable_alternative_buffer(bool value);
		static void set_raw_mode(bool value);
		static void set_cursor_visible(bool value);
		static void set_cursor_pos(size_t x, size_t y);
		static void reset_cursor_pos();

		static void clear();
		template<typename... arguments>
		static void write(arguments&&... args)
		{
			(std::cout << ... << args);
		}

		template<typename... arguments>
		static void writeln(arguments&&... args)
		{
			(std::cout << ... << args) << '\n';
		}

		static void erase(size_t count);

		static void set_title(const std::string& value);

		static std::tuple<size_t, size_t> size();
		*/
	};
}
