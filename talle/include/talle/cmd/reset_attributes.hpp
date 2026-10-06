#pragma once
#include <talle/cmd/command.hpp>
#include <talle/cmd/set_attribute.hpp>

namespace talle::cmd
{
	struct reset_attributes
	{
		reset_attributes() {}

		template<meta::output_writer writer>
		void write_ansi(writer& out) const
		{
			out << cmd::set_attribute{ style::attribute::reset };
		}
	};
}
