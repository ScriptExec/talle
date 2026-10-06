#include <talle/style/hyperlink.hpp>

namespace talle::style
{
	hyperlink::hyperlink(std::string_view url) : hyperlink{ std::string{ url } } {}
	hyperlink::hyperlink(const std::string& url) : content_{}, url_{ url } {}
	hyperlink::hyperlink(std::string_view content, std::string_view url) : hyperlink{ std::string{ content }, std::string{ url } } {}
	hyperlink::hyperlink(const std::string& content, const std::string& url) : content_{ content }, url_{ url } {}

	const std::string& hyperlink::content() const
	{
		return content_;
	}

	const std::string& hyperlink::content_or_url() const
	{
		return content_.empty() ? url_ : content_;
	}

	const std::string& hyperlink::url() const
	{
		return url_;
	}


}
