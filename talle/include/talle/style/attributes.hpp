#pragma once
#include <bitset>
#include <utility>
#include <talle/meta/writer.hpp>
#include <talle/style/attribute.hpp>

namespace talle::style
{
	class attributes
	{
	public:
		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = attribute;
			using difference_type = std::ptrdiff_t;
			using pointer = void;
			using reference = attribute;

			iterator(const attributes* owner, size_t index);

		private:
			const attributes* owner_;
			size_t index_;

		public:
			std::pair<attribute, bool> operator*() const;
			iterator& operator++();

			iterator operator++(int);
			friend bool operator==(const iterator& left, const iterator& right);
		private:
			void advance();
		};

		attributes() = default;
		attributes(std::initializer_list<attribute> attributes);

	private:
		std::bitset<detail::attribute_element_count> data_;

	public:
		attributes& set(attribute attr, bool value);
		attributes& add(attribute attr);
		attributes& remove(attribute attr);
		attributes& reset();

		bool has(attribute attr) const;
		bool empty() const;

		iterator begin() const;
		iterator end() const;
	private:
		static size_t map_attribute_to_idx(attribute attr);
	};

	template<meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const attributes& attr);
}
