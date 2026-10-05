#pragma once
#include <cstdint>
#include <variant>
#include <string>

namespace talle::style
{
	class color
	{
	public:
		enum class base : uint8_t
		{
			reset,
			black,
			dark_grey,
			red,
			dark_red,
			green,
			dark_green,
			yellow,
			dark_yellow,
			blue,
			dark_blue,
			magenta,
			dark_magenta,
			cyan,
			dark_cyan,
			white,
			grey,
		};

		struct ansi
		{
			uint8_t value{};
		};

		struct rgb
		{
			uint8_t r{};
			uint8_t g{};
			uint8_t b{};
		};

		color(const std::variant<base, ansi, rgb>& data) : data_{ data } {}
		color(const color& other) = default;
		color(color&& other) noexcept = default;

	private:
		std::variant<base, ansi, rgb> data_;

	public:
		std::string to_string() const
		{
			std::string result;
			if (auto base_col = try_get<base>())
			{
				switch (*base_col)
				{
					case base::black: result = "5;0"; break;
					case base::dark_grey: result = "5;8"; break;
					case base::red: result = "5;9"; break;
					case base::dark_red: result = "5;1"; break;
					case base::green: result = "5;10"; break;
					case base::dark_green: result = "5;2"; break;
					case base::yellow: result = "5;11"; break;
					case base::dark_yellow: result = "5;3"; break;
					case base::blue: result = "5;12"; break;
					case base::dark_blue: result = "5;4"; break;
					case base::magenta: result = "5;13"; break;
					case base::dark_magenta: result = "5;5"; break;
					case base::cyan: result = "5;14"; break;
					case base::dark_cyan: result = "5;6"; break;
					case base::white: result = "5;15"; break;
					case base::grey: result = "5;7"; break;
					case base::reset: [[fallthrough]];
					default: result = ""; break;
				}
			}
			else if (auto ansi_col = try_get<ansi>())
			{
				result = "5;" + std::to_string(ansi_col->value);
			}
			else if (auto rgb_col = try_get<rgb>())
			{
				result = "2;" + std::to_string(rgb_col->r) + ";" + std::to_string(rgb_col->g) + ";" + std::to_string(rgb_col->b);
			}
			return result;
		}

		bool is_reset() const
		{
			if (auto base_col = try_get<base>())
			{
				return *base_col == base::reset;
			}
			return false;
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

		color& operator=(const std::variant<base, ansi, rgb>& data)
		{
			data_ = data;
			return *this;
		}

		color& operator=(const color& other) = default;
		color& operator=(color&& other) noexcept = default;

		static color reset()
		{
			return color{ base::reset };
		}
	};
	
}
