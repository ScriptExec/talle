#pragma once
#include <ostream>
#include <concepts>
#include <functional>
#include <type_traits>

#include <talle/meta/writer.hpp>
#include <talle/meta/command.hpp>
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
			auto cmd = command{ std::forward<arguments>(args)... };
#ifdef _WIN32
			bool is_ansi_supported = has_ansi_support();
			if constexpr (meta::command_has_ansi_support_fn<command>)
			{
				is_ansi_supported = cmd.is_ansi_supported();
			}
			if (is_ansi_supported)
			{
				cmd.write_ansi(writer_.get());
			}
			else
			{
				if constexpr (meta::command_has_winapi_fn<command>)
				{
					cmd.call_winapi();
				}
			}
#else
			cmd.write_ansi(writer_.get());
#endif
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

		bool has_ansi_support() const
		{
			static const bool is_ansi_supported = []() -> bool
			{
#ifdef _WIN32
				if (sys::setup()) return true;

				auto term = env::get("TERM");
				return term.has_value() and term.value() != "dumb";
#else
				return true;
#endif
			}();
			return is_ansi_supported;
		}

		std::optional<position> get_cursor_pos()
		{
			return sys::get_cursor_pos();
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