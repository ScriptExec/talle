#pragma once
#include <memory>
#include <talle/utils/utf8_data.hpp>
#include <talle/render/frame_side.hpp>

namespace talle::impl
{
	class frame
	{
	public:
		frame(size_t hash) : hash_{ hash } {}
		virtual ~frame() = default;

	private:
		size_t hash_;

	public:
		constexpr size_t hash() const
		{
			return hash_;
		}

		virtual utf8_data get(frame_side sides) const = 0;
		virtual std::unique_ptr<frame> clone() const = 0;
	};
}
