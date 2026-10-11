#pragma once
#include <concepts>
#include <talle/utils/size.hpp>
#include <talle/utils/rect.hpp>
#include <talle/render/context.hpp>

namespace talle
{
	class widget
	{
	public:
		widget() : bounds_{}, visible_{ true } {}
		virtual ~widget() = default;

	protected:
		rect bounds_;
	private:
		bool visible_;

	public:
		const rect& bounds() const noexcept
		{
			return bounds_;
		}

		void set_visible(bool value) noexcept
		{
			visible_ = value;
		}

		bool visible() const noexcept
		{
			return visible_;
		}

		virtual size measure(size available) const = 0;

		virtual void arrange(const rect& bounds)
		{
			bounds_ = bounds;
		}

		virtual void render(render::context& ctx) const = 0;
	};

	namespace meta
	{
		template<typename type>
		concept widget = std::derived_from<type, talle::widget>;
	}
}
