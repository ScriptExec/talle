#pragma once
#include <term/utils/size.hpp>

namespace term
{
	struct resize_event
	{
		resize_event(size new_size) : size{ new_size } {}
		size size;
	};
}
