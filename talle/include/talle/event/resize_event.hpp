#pragma once
#include <talle/utils/size.hpp>

namespace talle
{
	struct resize_event
	{
		resize_event(size new_size) : size{ new_size } {}
		size size;
	};
}
