#pragma once
#include <ostream>
#include <concepts>
#include <type_traits>

namespace term::meta
{
	template<typename writer>
	concept output_writer = std::derived_from<writer, std::ostream>;

	template<typename writer, typename... arguments>
	concept writeable = requires(writer& out, arguments&&... args)
	{
		{ (out << ... << std::forward<arguments>(args)) } -> std::same_as<writer&>;
		{ out.flush() } -> std::same_as<writer&>;
	};
}
