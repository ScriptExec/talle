#pragma once
#include <ostream>
#include <concepts>
#include <type_traits>
#include <talle/meta/writer.hpp>

namespace talle::meta
{
	template<typename command_type, typename writer>
	concept command = requires(const command_type& cmd, writer& out)
	{
		{ cmd.template write_ansi<writer>(out) } -> std::same_as<void>;
	};

#ifdef _WIN32
	template<typename command_type>
	concept command_has_ansi_support_fn = requires(const command_type& cmd)
	{
		{ cmd.is_ansi_supported() } -> std::same_as<bool>;
	};

	template<typename command_type>
	concept command_has_winapi_fn = requires(const command_type& cmd)
	{
		{ cmd.call_winapi() } -> std::same_as<void>;
	};
#endif

	template<typename writer_type, typename... arguments>
	concept is_single_command = sizeof...(arguments) == 1 and (command<std::remove_cvref_t<arguments>, writer_type> and ...);
}
