#pragma once
#include <string>
#include <variant>
#include <ostream>

#include "color.hpp"
#include <talle/sys/platform.hpp>

namespace talle::style
{
	class color_part
	{
	public:
		struct foreground
		{
			color value;

			std::string to_string() const
			{
				if (value.is_reset()) return "39";
				return "38;" + value.to_string();
			}

			static foreground reset()
			{
				return foreground{ color::reset() };
			}

			friend std::ostream& operator<<(std::ostream& out, const foreground& col)
			{
				return out << "\x1b[" << col.to_string() << "m";
			}
		};

		struct background
		{
			color value;

			std::string to_string() const
			{
				if (value.is_reset()) return "49";
				return "48;" + value.to_string();
			}

			static background reset()
			{
				return background{ color::reset() };
			}

			friend std::ostream& operator<<(std::ostream& out, const background& col)
			{
				return out << "\x1b[" << col.to_string() << "m";
			}
		};

		struct underline
		{
			color value;

			std::string to_string() const
			{
				if (value.is_reset()) return "59";
				return "58;" + value.to_string();
			}

			static underline reset()
			{
				return underline{ color::reset() };
			}

			friend std::ostream& operator<<(std::ostream& out, const underline& col)
			{
				return out << "\x1b[" << col.to_string() << "m";
			}
		};

		color_part(const std::variant<foreground, background, underline>& data) : data_{ data } {}
		color_part(const color_part& other) = default;
		color_part(color_part&& other) noexcept = default;

	private:
		std::variant<foreground, background, underline> data_;

	public:
		color_part as_reset() const
		{
			return std::visit([](const auto& part)
			{
				return color_part{ part.reset() };
			}, data_);
		}

		std::string to_string() const
		{
			return std::visit([](const auto& part)
			{
				return part.to_string();
			}, data_);
		}

		template<typename type>
		bool is() const
		{
			return std::holds_alternative<type>(data_);
		}

		template<typename type>
		type& get()
		{
			return std::get<type>(data_);
		}

		template<typename type>
		const type& get() const
		{
			return std::get<type>(data_);
		}

		template<typename type>
		type* try_get()
		{
			return std::get_if<type>(&data_);
		}

		template<typename type>
		const type* try_get() const
		{
			return std::get_if<type>(&data_);
		}

		color_part& operator=(const std::variant<foreground, background, underline>& data)
		{
			data_ = data;
			return *this;
		}

		color_part& operator=(const color_part& other) = default;
		color_part& operator=(color_part&& other) noexcept = default;

		friend std::ostream& operator<<(std::ostream& out, const color_part& part)
		{
			if (!sys::is_color_enabled()) return out;
			return out << part;
		}
	};
}
