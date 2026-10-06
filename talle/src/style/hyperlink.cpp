#include <talle/style/hyperlink.hpp>

namespace talle::style
{
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
