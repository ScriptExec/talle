#pragma once
#include <concepts>
#include <ostream>
#include <functional>
#include <type_traits>

#include <talle/meta/writer.hpp>
#include <talle/cmd/command.hpp>
#include <talle/sys/platform.hpp>

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
			if (has_ansi_support() and cmd.is_ansi_supported())
			{
				cmd.write_ansi(writer_.get());
			}
			else
			{
				cmd.call_winapi();
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
#ifdef _MSC_VER
				char* term = nullptr;
				std::size_t required_size = 0;
				if (_dupenv_s(&term, &required_size, "TERM") != 0) term = nullptr;
#else
				const char* term = std::getenv("TERM");
#endif
				return term != nullptr and std::strcmp(term, "dumb") != 0;
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