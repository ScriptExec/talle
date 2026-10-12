#pragma once
#include <memory>
#include <vector>
#include <typeinfo>
#include <unordered_map>
#include <talle/render/draw_buffer.hpp>
#include <talle/render/frame/impl/frame.hpp>
#include <talle/meta/frame.hpp>
#include <talle/render/frame/none.hpp>

namespace talle::render
{
	struct context
	{
		draw_buffer& output;
		std::unordered_map<size_t, std::unique_ptr<impl::frame>> frame_types;
		std::unordered_map<size_t, std::vector<rect>> frames;

		template<meta::frame frame_type>
		void add_frame(const rect& rect)
		{
			auto hash = typeid(frame_type).hash_code();
			if constexpr (std::is_same_v<frame_type, frame::none>)
			{
				frame_types[hash] = nullptr;
			}
			else
			{
				frame_types[hash] = std::make_unique<frame_type>(hash);
			}
			frames[hash].push_back(rect);
		}

		void add_frame(const impl::frame& frame, const rect& rect)
		{
			auto hash = frame.hash();
			auto& frame_type = frame_types[hash];
			if (frame_type == nullptr)
			{
				frame_type = frame.clone();
			}
			frames[hash].push_back(rect);
		}
	};
}
