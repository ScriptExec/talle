#include <array>
#include <string>
#include <cstdint>

namespace talle
{
	struct utf8_data
	{
		constexpr utf8_data(uint8_t c = 0) : data{ c, 0, 0, 0 } {}
		constexpr utf8_data(const std::string& utf8str) : data{ 0, 0, 0, 0 }
		{
			for (size_t i = 0; i < utf8str.size() and i < data.size(); ++i)
			{
				data[i] = static_cast<uint8_t>(utf8str[i]);
			}
		}
		constexpr utf8_data(const char* utf8str) : data{ 0, 0, 0, 0 }
		{
			for (size_t i = 0; utf8str[i] != '\0' and i < data.size(); ++i)
			{
				data[i] = static_cast<uint8_t>(utf8str[i]);
			}
		}

		std::array<uint8_t, 4> data{};

		constexpr utf8_data& operator=(uint8_t c)
		{
			data = { c, 0, 0, 0 };
			return *this;
		}

		constexpr utf8_data& operator=(const std::string& utf8str)
		{
			data = { 0, 0, 0, 0 };
			for (size_t i = 0; i < utf8str.size() and i < data.size(); ++i)
			{
				data[i] = static_cast<uint8_t>(utf8str[i]);
			}
			return *this;
		}

		constexpr utf8_data& operator=(std::string_view utf8str_view)
		{
			data = { 0, 0, 0, 0 };
			for (size_t i = 0; i < utf8str_view.size() and i < data.size(); ++i)
			{
				data[i] = static_cast<uint8_t>(utf8str_view[i]);
			}
			return *this;
		}

		constexpr uint8_t& operator[](size_t index)
		{
			return data.at(index);
		}

		constexpr const uint8_t& operator[](size_t index) const
		{
			return data.at(index);
		}

		constexpr bool operator==(uint8_t c) const
		{
			return data[0] == c and data[1] == 0 and data[2] == 0 and data[3] == 0;
		}
	};
}