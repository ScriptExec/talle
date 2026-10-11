#pragma once
#include <vector>
#include <unordered_map>
#include <talle/render/draw_buffer.hpp>
#include <talle/render/frame/type.hpp>

namespace talle::render
{
	struct context
	{
		draw_buffer& output;
		std::unordered_map<frame::type, std::vector<rect>> frames;
	};
}
