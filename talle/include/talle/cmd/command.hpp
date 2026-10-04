#pragma once
#include <ostream>
#include <concepts>
#include <talle/meta/writer.hpp>

namespace talle::meta
{
	template<typename command_type, typename writer>
	concept command = requires(const command_type& cmd, writer& out)
	{
		{ cmd.template write_ansi<writer>(out) } -> std::same_as<void>;
#ifdef _WIN32
		{ cmd.call_winapi() } -> std::same_as<void>;
		{ cmd.is_ansi_supported() } -> std::same_as<bool>;
#endif
	};
}
