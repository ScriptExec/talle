#include <talle/sys/platform.hpp>
#include <atomic>

#ifdef _WIN32
	#include "windows.hpp"
#else
	#include "unix.hpp"
#endif

#include <talle/utils/env.hpp>

namespace talle::sys
{
	std::atomic<bool> color_enabled{ true };
	std::atomic<bool> supports_ansi{ false };

	bool has_ansi_support()
	{
		static auto initialized = []()
		{
			auto term = env::get("TERM");
			bool result = sys::setup() or (term.has_value() and term.value() != "dumb");
			supports_ansi.store(result);
			return result;
		}();
		return supports_ansi.load();
	}

	bool is_color_enabled()
	{
		static auto initialized = []()
		{
			if (env::get("NO_COLOR").has_value())
			{
				set_color_enabled(false);
				return true;
			}
			if (env::get("FORCE_COLOR").has_value())
			{
				set_color_enabled(true);
				return true;
			}
			return false;
		}();
		return color_enabled.load();
	}

	bool set_color_enabled(bool value)
	{
		color_enabled.store(value);
		return true;
	}
}
