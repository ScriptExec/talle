#pragma once
#include <ostream>
#include <concepts>
#include <functional>
#include <type_traits>

#include <talle/meta/writer.hpp>
#include <talle/cmd/command.hpp>
#include <talle/sys/platform.hpp>
#include <talle/utils/env.hpp>

namespace talle
{
	template<meta::output_writer writer>
	class backend
	{
	public:
		using writer_type = writer;
		explicit backend(writer_type& out) : writer_{ out } {}

	private:
		std::reference_wrapper<writer_type> writer_;

	public:
		template<meta::command<writer_type> command, typename... arguments>
		void execute(arguments&&... args)
		{
			auto comm = command{ std::forward<arguments>(args)... };
			writer_.get() << comm;
		}

		template<typename... arguments> requires meta::writeable<writer_type, arguments...>
		void write(arguments&&... value)
		{
			(writer_.get() << ... << std::forward<arguments>(value));
		}

		template<typename... arguments> requires meta::writeable<writer_type, arguments...>
		void writeln(arguments&&... value)
		{
			(writer_.get() << ... << std::forward<arguments>(value)) << '\n';
		}

		void flush()
		{
			writer_.get().flush();
		}

		bool set_raw_mode(bool value)
		{
			return sys::set_raw_mode(value);
		}

		bool is_raw_mode_enabled() const
		{
			return sys::is_raw_mode_enabled();
		}

		std::optional<position> get_cursor_pos()
		{
			return sys::get_cursor_pos();
		}

		std::optional<size> get_size()
		{
			return sys::get_size();
		}
	};

	namespace meta
	{
		template<typename backend_type>
		concept backend = requires
		{
			typename backend_type::writer_type;
		}
		and std::derived_from<backend_type, talle::backend<typename backend_type::writer_type>>;
	}
}