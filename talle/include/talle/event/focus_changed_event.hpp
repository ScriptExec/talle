#pragma once
#include <talle/utils/size.hpp>

namespace talle
{
	enum class focus_change_type
	{
		gained,
		lost
	};

	struct focus_changed_event
	{
		focus_changed_event(focus_change_type type) : type{ type } {}

		focus_change_type type;
	};
}
