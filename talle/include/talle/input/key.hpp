#pragma once
#include <cstdint>
#include <variant>

namespace talle
{
	class key
	{
	public:
		enum class code : uint8_t
		{
			unknown,
			backspace,
			enter,
			left,
			right,
			up,
			down,
			home,
			end,
			pageup,
			pagedown,
			tab,
			backtab,
			delete_,
			insert,
			escape,
		};

		struct fn
		{
			uint8_t number{};

			bool operator==(const fn& other) const
			{
				return number == other.number;
			}
		};

		struct chr
		{
			uint8_t c{};

			bool operator==(const chr& other) const
			{
				return c == other.c;
			}
		};
		
		key(code c) : data_{ c } {}
		key(fn f) : data_{ f } {}
		key(chr c) : data_{ c } {}

	private:
		std::variant<code, fn, chr> data_;

	public:
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

		bool operator==(const key& other) const
		{
			return data_ == other.data_;
		}

		bool operator==(const code& other) const
		{
			auto val = try_get<code>();
			return val and *val == other;
		}

		bool operator==(const fn& other) const
		{
			auto val = try_get<fn>();
			return val and *val == other;
		}

		bool operator==(const chr& other) const
		{
			auto val = try_get<chr>();
			return val and *val == other;
		}

		bool operator!=(const key& other) const
		{
			return !(*this == other);
		}

		bool operator!=(const code& other) const
		{
			return !(*this == other);
		}

		bool operator!=(const fn& other) const
		{
			return !(*this == other);
		}

		bool operator!=(const chr& other) const
		{
			return !(*this == other);
		}
	};
}
