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
	};
}
