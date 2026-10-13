#pragma once
#include <talle/utils/utf8_data.hpp>
#include <talle/style/style.hpp>

namespace talle
{
	class cell
	{
	public:
		cell(const utf8_data& data = { ' ' }, const style::style& style = {}) : data_{ data }, style_{ style } {}

	private:
		utf8_data data_;
		style::style style_;

	public:
		cell& set_data(const utf8_data& data)
		{
			data_ = data;
			return *this;
		}

		cell& set_style(const style::style& style)
		{
			style_ = style;
			return *this;
		}

		utf8_data& data()
		{
			return data_;
		}

		const utf8_data& data() const
		{
			return data_;
		}

		style::style& style()
		{
			return style_;
		}

		const style::style& style() const
		{
			return style_;
		}

		bool operator==(const cell& other) const
		{
			return data_ == other.data() and style_ == other.style();
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
