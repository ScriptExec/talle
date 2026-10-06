#pragma once
#include <string>
#include <optional>
#include <concepts>
#include <type_traits>
#include <string_view>
#include <variant>
#include <talle/meta/writer.hpp>
#include <talle/meta/content.hpp>

namespace talle::style
{
	template<meta::content url_type, meta::content content_type = std::string>
	class hyperlink
	{
	public:
		using stored_url_type = meta::content_decay_type<url_type>;
		using stored_content_type = meta::content_decay_type<content_type>;

		hyperlink(const stored_url_type& url) : content_{}, url_{ url } {}
		hyperlink(const stored_url_type& url, const stored_content_type& content) : content_{ content }, url_{ url } {}
		hyperlink(const hyperlink&) = default;
		hyperlink(hyperlink&&) = default;

	private:
		std::optional<stored_content_type> content_;
		stored_url_type url_;

	public:
		const std::optional<stored_content_type>& content() const
		{
			return content_;
		}

		///@return Content if present, URL otherwise
		auto content_or_url() const
		{
			if constexpr (std::same_as<stored_url_type, stored_content_type>)
			{
				return content_.value_or(url_);
			}
			else
			{
				if (content_.has_value()) return std::variant<stored_url_type, stored_content_type>{ *content_ };
				return std::variant<stored_url_type, stored_content_type>{ url_ };
			}
		}

		const stored_url_type& url() const
		{
			return url_;
		}

		hyperlink& operator=(const hyperlink&) = default;
		hyperlink& operator=(hyperlink&&) = default;
	};

	template<meta::content url_type>
	hyperlink(const url_type&) -> hyperlink<url_type>;

	template<meta::content url_type, meta::content content_type>
	hyperlink(const url_type&, const content_type&) -> hyperlink<url_type, content_type>;

	template<meta::output_writer writer_type, meta::content url_type, meta::content content_type>
	writer_type& operator<<(writer_type& writer, const hyperlink<url_type, content_type>& link);
}
