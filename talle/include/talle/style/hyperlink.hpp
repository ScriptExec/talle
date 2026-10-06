#pragma once
#include <string>
#include <concepts>
#include <type_traits>
#include <string_view>
#include <talle/meta/writer.hpp>

namespace talle::style
{
	class hyperlink
	{
	public:
		template<typename type> requires std::constructible_from<std::string, type>
		hyperlink(const type& url) : url_{ url } {}
		template<typename content_type, typename url_type> requires std::constructible_from<std::string, content_type> and std::constructible_from<std::string, url_type>
		hyperlink(const content_type& content, const url_type& url) : content_{ content }, url_{ url } {}
		hyperlink(const hyperlink&) = default;
		hyperlink(hyperlink&&) = default;

	private:
		std::string content_;
		std::string url_;

	public:
		const std::string& content() const;
		///@return Content if it's not empty, URL otherwise
		const std::string& content_or_url() const;
		const std::string& url() const;

		hyperlink& operator=(const hyperlink&) = default;
		hyperlink& operator=(hyperlink&&) = default;
	};

	template<meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const hyperlink& link);
}
