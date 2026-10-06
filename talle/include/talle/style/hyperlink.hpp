#pragma once
#include <string>
#include <string_view>
#include <talle/meta/writer.hpp>

namespace talle::style
{
	class hyperlink
	{
	public:
		hyperlink(std::string_view url);
		hyperlink(const std::string& url);
		hyperlink(std::string_view content, std::string_view url);
		hyperlink(const std::string& content, const std::string& url);

	private:
		std::string content_;
		std::string url_;

	public:
		const std::string& content() const;
		///@return Content if it's not empty, URL otherwise
		const std::string& content_or_url() const;
		const std::string& url() const;
	};

	template<meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const hyperlink& link);
}
