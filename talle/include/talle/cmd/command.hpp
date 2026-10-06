#pragma once
#include <ostream>
#include <talle/meta/command.hpp>
#include <talle/sys/platform.hpp>
#include <talle/meta/writer.hpp>

namespace talle::cmd
{
	template<meta::output_writer writer_type, meta::command<writer_type> command_type, typename... arguments>
	void execute(writer_type& writer, arguments&&... args)
	{
		auto comm = command_type{ std::forward<arguments>(args)... };
		execute<writer_type, command_type>(writer, comm);
	}

	template<meta::output_writer writer_type, meta::command<writer_type> command_type>
	void execute(writer_type& writer, const command_type& comm)
	{
		bool is_ansi_supported = sys::has_ansi_support();
#ifdef _WIN32
		if constexpr (meta::command_has_ansi_support_fn<command_type>)
		{
			is_ansi_supported = comm.is_ansi_supported();
		}
		if (!is_ansi_supported)
		{
			if constexpr (meta::command_has_winapi_fn<command_type>)
			{
				comm.call_winapi();
			}
			return;
		}
#endif
		comm.write_ansi(writer);
	}

	template<meta::output_writer writer_type, meta::command<writer_type> command_type>
	writer_type& operator<<(writer_type& writer, const command_type& comm)
	{
		execute<writer_type, command_type>(writer, comm);
		return writer;
	}
}
