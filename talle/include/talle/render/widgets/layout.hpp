#pragma once
#include <vector>
#include <tuple>
#include <memory>
#include <talle/render/direction.hpp>
#include <talle/render/widgets/widget.hpp>
#include <talle/render/layout/flex.hpp>
#include <talle/render/layout/alignment.hpp>
#include <talle/render/layout/constraints.hpp>
#include <talle/render/layout/padding.hpp>
#include <talle/render/frame/type.hpp>

namespace talle
{
	class layout : public widget
	{
	public:
		struct child_layout
		{
			float grow_factor = 0.0f;
			float shrink_factor = 0.0f;
		};

		template<meta::widget widget_type>
		struct child
		{
			std::unique_ptr<widget_type> item;
			constraints constraints;
			child_layout layout;
		};

		explicit layout(direction dir) : direction_{ dir }, gap_{ 0 }, flex_{ flex::start }, alignment_{ alignment::start }, frame_type_{ frame::type::none } {}

	private:
		std::vector<std::unique_ptr<child<widget>>> children_;
		padding padding_;
		uint16_t gap_;
		frame::type frame_type_;
		flex flex_;
		alignment alignment_;
		direction direction_;

	public:
		layout& set_flex(flex value);
		layout& set_alignment(alignment value);
		layout& set_padding(const padding& value);
		layout& set_direction(direction value);
		layout& set_gap(uint16_t value);
		layout& set_frame_type(frame::type value);

		rect inner_bounds() const;

		virtual size measure(size available) const override;
		virtual void arrange(const rect& bounds) override;
		virtual void render(render::context& ctx) const override;

		template<meta::widget widget_type, typename... arguments> requires std::constructible_from<widget_type, arguments...>
		child<widget_type>& add(arguments&&... args)
		{
			auto child_widget = std::make_unique<widget_type>(std::forward<arguments>(args)...);
			auto ptr = std::make_unique<child<widget>>(std::move(child_widget), constraints{}, child_layout{});
			auto raw_ptr = ptr.get();
			children_.emplace_back(std::move(ptr));
			return *reinterpret_cast<child<widget_type>*>(raw_ptr);
		}
	};
}
