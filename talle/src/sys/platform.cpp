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
