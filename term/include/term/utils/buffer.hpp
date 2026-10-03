#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <tuple>

namespace term
{
	template<typename type>
	struct buffer
	{
	public:
		buffer() : width_{}, height_{} {}
		buffer(size_t width, size_t height)
		{
			width_ = width;
			height_ = height;
			resize(width, height);
		}

	private:
		std::vector<type> data_;
		size_t width_;
		size_t height_;

	public:
		void resize(size_t width, size_t height)
		{
			data_.resize(width * height);
		}

		void fill(type value)
		{
			for (auto& c : data_)
			{
				c = value;
			}
		}

		void replace(size_t x, size_t y, const std::string& value)
		{
			if (x >= width_ || y >= height_) return;

			size_t index = y * width_ + x;
			for (size_t i = 0; i < value.size() and (index + i) < data_.size(); ++i)
			{
				data_[index + i] = static_cast<type>(value[i]);
			}
		}

		void insert(size_t x, size_t y, const std::string& value)
		{
			//if (x >= width_ || y >= height_) return;
			size_t index = y * width_ + x;
			for (size_t i = 0; i < value.size(); ++i)
			{
				data_.insert(data_.begin() + index + i, static_cast<type>(value[i]));
			}
		}

		std::tuple<size_t, size_t> size() const
		{
			return std::make_tuple(width_, height_);
		}

		size_t width() const
		{
			return width_;
		}

		size_t height() const
		{
			return height_;
		}

		std::vector<type>& data()
		{
			return data_;
		}

		type& operator[](size_t index)
		{
			return data_.at(index);
		}

		const type& operator[](size_t index) const
		{
			return data_.at(index);
		}

		type& operator()(size_t x, size_t y)
		{
			return data_.at(y * width_ + x);
		}

		const type& operator()(size_t x, size_t y) const
		{
			return data_.at(y * width_ + x);
		}
	};
}
