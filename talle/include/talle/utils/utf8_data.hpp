#pragma once
#include <array>
#include <string>
#include <string_view>
#include <cstdint>

#include <talle/meta/writer.hpp>

namespace talle
{
	class utf8_data
	{
	public:
		constexpr utf8_data(uint8_t c = 0) : data_{ c, 0, 0, 0 } {}

		constexpr utf8_data(const std::string& utf8str) : data_{ 0, 0, 0, 0 }
		{
			for (size_t i = 0; i < data_.size() and i < utf8str.size(); ++i)
			{
				data_[i] = static_cast<uint8_t>(utf8str[i]);
			}
		}

		constexpr utf8_data(const std::u8string& utf8str) : data_{ 0, 0, 0, 0 }
		{
			for (size_t i = 0; i < data_.size() and i < utf8str.size(); ++i)
			{
				data_[i] = static_cast<uint8_t>(utf8str[i]);
			}
		}

		constexpr utf8_data(std::string_view utf8str) : data_{ 0, 0, 0, 0 }
		{
			for (size_t i = 0; i < data_.size() and i < utf8str.size(); ++i)
			{
				data_[i] = static_cast<uint8_t>(utf8str[i]);
			}
		}

		constexpr utf8_data(std::u8string_view utf8str) : data_{ 0, 0, 0, 0 }
		{
			for (size_t i = 0; i < data_.size() and i < utf8str.size(); ++i)
			{
				data_[i] = static_cast<uint8_t>(utf8str[i]);
			}
		}

		constexpr utf8_data(const char* utf8str) : data_{ 0, 0, 0, 0 }
		{
			for (size_t i = 0; i < data_.size() and utf8str[i] != '\0'; ++i)
			{
				data_[i] = static_cast<uint8_t>(utf8str[i]);
			}
		}

		constexpr utf8_data(const char8_t* utf8str) : data_{ 0, 0, 0, 0 }
		{
			for (size_t i = 0; i < data_.size() and utf8str[i] != u8'\0'; ++i)
			{
				data_[i] = static_cast<uint8_t>(utf8str[i]);
			}
		}

	private:
		std::array<uint8_t, 4> data_{};

	public:
		constexpr const std::array<uint8_t, 4>& data() const
		{
			return data_;
		}

		constexpr std::array<uint8_t, 4>& data()
		{
			return data_;
		}

		constexpr utf8_data& operator=(uint8_t c)
		{
			data_ = { c, 0, 0, 0 };
			return *this;
		}

		constexpr utf8_data& operator=(const std::string& utf8str)
		{
			data_ = { 0, 0, 0, 0 };
			for (size_t i = 0; i < utf8str.size() and i < data_.size(); ++i)
			{
				data_[i] = static_cast<uint8_t>(utf8str[i]);
			}
			return *this;
		}

		constexpr utf8_data& operator=(const std::u8string& utf8str)
		{
			data_ = { 0, 0, 0, 0 };
			for (size_t i = 0; i < data_.size() and i < utf8str.size(); ++i)
			{
				data_[i] = static_cast<uint8_t>(utf8str[i]);
			}
			return *this;
		}

		constexpr utf8_data& operator=(std::string_view utf8str_view)
		{
			data_ = { 0, 0, 0, 0 };
			for (size_t i = 0; i < data_.size() and i < utf8str_view.size(); ++i)
			{
				data_[i] = static_cast<uint8_t>(utf8str_view[i]);
			}
			return *this;
		}

		constexpr utf8_data& operator=(std::u8string_view utf8str_view)
		{
			data_ = { 0, 0, 0, 0 };
			for (size_t i = 0; i < data_.size() and i < utf8str_view.size(); ++i)
			{
				data_[i] = static_cast<uint8_t>(utf8str_view[i]);
			}
			return *this;
		}

		constexpr uint8_t& operator[](size_t index)
		{
			return data_.at(index);
		}

		constexpr const uint8_t& operator[](size_t index) const
		{
			return data_.at(index);
		}

		constexpr bool operator==(uint8_t c) const
		{
			return data_[0] == c and data_[1] == 0 and data_[2] == 0 and data_[3] == 0;
		}

		constexpr bool operator==(const utf8_data& other) const
		{
			return data_ == other.data_;
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
		for (const auto& byte : utf8.data())
		{
			if (byte == 0) break;
			writer << byte;
		}
		return writer;
	}
}