#pragma once
#include <string>
#include <concepts>
#include <type_traits>
#include "style.hpp"

#include <talle/meta/content.hpp>
#include <talle/sys/platform.hpp>

namespace talle::style
{
	template<meta::content content_type>
	class styled
	{
	public:
		using stored_content_type = meta::content_decay_type<content_type>;

		template<typename... arguments> requires std::constructible_from<stored_content_type, arguments...>
		styled(arguments&&... args) : content_{ std::forward<arguments>(args)... } {};
		styled(const stored_content_type& content) : content_{ content } {};

	private:
		stored_content_type content_;
		style style_;

	public:
		styled& set_foreground(const std::optional<color>& color)
		{
			style_.set_foreground(color);
			return *this;
		}

		styled& reset_foreground()
		{
			style_.reset_foreground();
			return *this;
		}

		styled& set_background(const std::optional<color>& color)
		{
			style_.set_background(color);
			return *this;
		}

		styled& reset_background()
		{
			style_.reset_background();
			return *this;
		}

		styled& set_underline(const std::optional<color>& color)
		{
			style_.set_underline(color);
			return *this;
		}

		styled& reset_underline()
		{
			style_.reset_underline();
			return *this;
		}

		styled& set(attribute attr, bool value)
		{
			style_.set(attr, value);
			return *this;
		}

		styled& add(attribute attr)
		{
			style_.add(attr);
			return *this;
		}

		styled& remove(attribute attr)
		{
			style_.remove(attr);
			return *this;
		}

		styled& reset()
		{
			style_.reset();
			return *this;
		}

		const content_type& content() const
		{
			return content_;
		}

		const style& get_style() const
		{
			return style_;
		}
	};

	template<meta::content content_type>
	styled(const content_type&) -> styled<content_type>;

	template<meta::output_writer writer_type, meta::content content_type>
	writer_type& operator<<(writer_type& writer, const styled<content_type>& content);
}
