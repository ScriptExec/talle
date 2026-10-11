#pragma once
#include <cstdint>
#include <optional>

namespace talle
{
	class constraints
	{
	public:
		std::optional<uint16_t> min_width;
		std::optional<uint16_t> min_height;

		std::optional<uint16_t> max_width;
		std::optional<uint16_t> max_height;

		std::optional<uint16_t> width;
		std::optional<uint16_t> height;

		std::optional<float> width_percentage;
		std::optional<float> height_percentage;

	private:
		bool fill_ = false;

	public:
		constraints& fill_space()
		{
			fill_ = true;
			return *this;
		}

		bool fill() const
		{
			return fill_;
		}
	};
}
