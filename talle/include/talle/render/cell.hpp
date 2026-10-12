#pragma once
#include <talle/utils/utf8_data.hpp>

namespace talle
{
	class cell
	{
	public:
		cell(const utf8_data& data = { ' ' }) : data_{data} {}

	private:
		utf8_data data_;

	public:
		utf8_data& data()
		{
			return data_;
		}

		const utf8_data& data() const
		{
			return data_;
		}

		///@return Empty cell filled with whitespace
		static cell empty()
		{
			return cell{};
		}

		///@return Cell which should be ignored by the renderer
		static cell dont_care()
		{
			return cell{ utf8_data{ uint8_t{} } };
		}
	};
}
