#pragma once
#include <array>
#include <string>
#include <string_view>
#include <cstdint>

#include <talle/meta/writer.hpp>

namespace talle
{
	struct utf8_data
	{
		constexpr utf8_data(uint8_t c = 0) : data{ c, 0, 0, 0 } {}

		constexpr utf8_data(const std::string& utf8str) : data{ 0, 0, 0, 0 }
		{
			for (size_t i = 0; i < data.size() and i < utf8str.size(); ++i)
			{
				data[i] = static_cast<uint8_t>(utf8str[i]);
			}
		}

		constexpr utf8_data(const std::u8string& utf8str) : data{ 0, 0, 0, 0 }
		{
			for (size_t i = 0; i < data.size() and i < utf8str.size(); ++i)
			{
				data[i] = static_cast<uint8_t>(utf8str[i]);
			}
		}

		constexpr utf8_data(std::string_view utf8str) : data{ 0, 0, 0, 0 }
		{
			for (size_t i = 0; i < data.size() and i < utf8str.size(); ++i)
			{
				data[i] = static_cast<uint8_t>(utf8str[i]);
			}
		}

		constexpr utf8_data(std::u8string_view utf8str) : data{ 0, 0, 0, 0 }
		{
			for (size_t i = 0; i < data.size() and i < utf8str.size(); ++i)
			{
				data[i] = static_cast<uint8_t>(utf8str[i]);
			}
		}

		constexpr utf8_data(const char* utf8str) : data{ 0, 0, 0, 0 }
		{
			for (size_t i = 0; i < data.size() and utf8str[i] != '\0'; ++i)
			{
				data[i] = static_cast<uint8_t>(utf8str[i]);
			}
		}

		constexpr utf8_data(const char8_t* utf8str) : data{ 0, 0, 0, 0 }
		{
			for (size_t i = 0; i < data.size() and utf8str[i] != u8'\0'; ++i)
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

		constexpr utf8_data& operator=(const std::u8string& utf8str)
		{
			data = { 0, 0, 0, 0 };
			for (size_t i = 0; i < data.size() and i < utf8str.size(); ++i)
			{
				data[i] = static_cast<uint8_t>(utf8str[i]);
			}
			return *this;
		}

		constexpr utf8_data& operator=(std::string_view utf8str_view)
		{
			data = { 0, 0, 0, 0 };
			for (size_t i = 0; i < data.size() and i < utf8str_view.size(); ++i)
			{
				data[i] = static_cast<uint8_t>(utf8str_view[i]);
			}
			return *this;
		}

		constexpr utf8_data& operator=(std::u8string_view utf8str_view)
		{
			data = { 0, 0, 0, 0 };
			for (size_t i = 0; i < data.size() and i < utf8str_view.size(); ++i)
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

		constexpr bool operator==(const utf8_data& other) const
		{
			return data == other.data;
		}

		constexpr bool operator!=(uint8_t c) const
		{
			return !(*this == c);
		}

		constexpr bool operator!=(const utf8_data& other) const
		{
			return !(*this == other);
		}
	};

	template<meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const utf8_data& utf8)
	{
		for (const auto& byte : utf8.data)
		{
			if (byte == 0) break;
			writer << byte;
		}
		return writer;
	}
}