#include <talle/style/attributes.hpp>

namespace talle::style
{
	attributes::iterator::iterator(const attributes* owner, size_t index) : owner_{ owner }, index_{ index }
	{
		advance();
	}

	std::pair<attribute, bool> attributes::iterator::operator*() const
	{
		return std::make_pair(static_cast<attribute>(index_), owner_->data_[index_]);
	}

	attributes::iterator& attributes::iterator::operator++()
	{
		++index_;
		advance();
		return *this;
	}

	attributes::iterator attributes::iterator::operator++(int)
	{
		auto tmp = *this;
		++(*this);
		return tmp;
	}

	bool operator==(const attributes::iterator& left, const attributes::iterator& right)
	{
		return left.owner_ == right.owner_ and left.index_ == right.index_;
	}

	void attributes::iterator::advance()
	{
		while (index_ < detail::attribute_element_count and !owner_->data_[index_])
		{
			++index_;
		}
	}

	attributes::attributes(std::initializer_list<attribute> attributes)
	{
		for (auto attr : attributes)
		{
			add(attr);
		}
	}

	attributes& attributes::set(attribute attr, bool value)
	{
		data_.set(map_attribute_to_idx(attr), value);
		return *this;
	}

	attributes& attributes::add(attribute attr)
	{
		return set(attr, true);
	}

	attributes& attributes::remove(attribute attr)
	{
		return set(attr, false);
	}

	attributes& attributes::reset()
	{
		data_.reset();
		return *this;
	}

	bool attributes::has(attribute attr) const
	{
		return data_.test(map_attribute_to_idx(attr));
	}

	bool attributes::empty() const
	{
		return data_.none();
	}

	attributes::iterator attributes::begin() const
	{
		return attributes::iterator{ this, 0 };
	}

	attributes::iterator attributes::end() const
	{
		return attributes::iterator{ this, detail::attribute_element_count };
	}

	size_t attributes::map_attribute_to_idx(attribute attr)
	{
		return static_cast<size_t>(attr);
	}
}
