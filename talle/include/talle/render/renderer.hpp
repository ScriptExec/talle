#pragma once
#include <memory>
#include <optional>
#include <vector>
#include <map>

#include <talle/render/presenters/immediate_presenter.hpp>
#include <talle/meta/frame_style.hpp>
#include <talle/core/backend.hpp>
#include <talle/utils/rect.hpp>
#include <talle/render/widgets/widget.hpp>
#include <talle/render/frame/frames.hpp>

#include "present_mode.hpp"
#include "context.hpp"
#include "cell.hpp"
#include "presenter.hpp"

namespace talle
{
	template<meta::backend backend_type>
	class renderer
	{
	public:
		renderer(backend_type& backend) : backend_{ backend }
		{
			set_presenter<immediate_presenter<backend_type>>();
		}
	private:
		backend_type& backend_;
		size active_size_;
		std::unique_ptr<presenter<backend_type>> presenter_;

	public:
		void resize(size new_size)
		{
			active_size_ = new_size;

			if (presenter_ == nullptr) return;
			presenter_->on_resize(active_size_);
		}

		void clear(const std::optional<cell>& fill_cell = std::nullopt)
		{
			if (presenter_ == nullptr) return;
			presenter_->on_clear(fill_cell);
		}

		void clear(const rect& area, const std::optional<cell>& fill_cell = std::nullopt)
		{
			if (presenter_ == nullptr) return;
			presenter_->on_clear(area, fill_cell);
		}

		void render()
		{
			if (presenter_ == nullptr) return;

			presenter_->on_render();
		}

		void render(position pos, const cell& cell)
		{
			if (presenter_ == nullptr) return;
			presenter_->set(pos, cell);
		}

		template<meta::frame_style frame_style_type>
		void frame(const rect& box)
		{
			if (presenter_ == nullptr) return;

			frame_style_type style{};
			//TODO: Check if size isn't out of terminal bounds
			for (uint16_t y = box.start.y; y <= box.end.y; y++)
			{
				for (uint16_t x = box.start.x; x <= box.end.x; x++)
				{
					const bool is_top = y == box.start.y;
					const bool is_bottom = y == box.end.y;
					const bool left = (is_top or is_bottom) and (x <= box.end.x and x != box.start.x);
					const bool right = (is_top or is_bottom) and (x >= box.start.x and x != box.end.x);
					const bool top = !is_top and (x == box.start.x or x == box.end.x);
					const bool bottom = !is_bottom and (x == box.start.x or x == box.end.x);

					if (!top and !bottom and !left and !right) continue;

					frame_side sides{};

					if (left) sides |= frame_side::left;
					if (right) sides |= frame_side::right;
					if (top) sides |= frame_side::top;
					if (bottom) sides |= frame_side::bottom;

					render(position{ x, y }, cell{ style.get(sides) });
				}
			}
		}

		template<meta::frame_style frame_style_type>
		void frames(const std::vector<rect>& boxes)
		{
			if (presenter_ == nullptr) return;

			frame_style_type style{};
			std::map<position, frame_side> borders;

			for (const rect& box : boxes)
			{
				const auto is_border = [&](uint16_t x, uint16_t y)
				{
					return x >= box.start.x and x <= box.end.x and y >= box.start.y and y <= box.end.y and
						(x == box.start.x or x == box.end.x or y == box.start.y or y == box.end.y);
				};

				for (uint16_t y = box.start.y; y <= box.end.y; ++y)
				{
					for (uint16_t x = box.start.x; x <= box.end.x; ++x)
					{
						if (!is_border(x, y)) continue;

						frame_side sides{};

						if (x > box.start.x and is_border(x - 1, y)) sides |= frame_side::left;
						if (x < box.end.x and is_border(x + 1, y)) sides |= frame_side::right;
						if (y > box.start.y and is_border(x, y - 1)) sides |= frame_side::top;
						if (y < box.end.y and is_border(x, y + 1)) sides |= frame_side::bottom;

						borders[{x, y}] |= sides;
					}
				}
			}

			for (const auto& [pos, sides] : borders)
			{
				render(position{ pos.x, pos.y }, cell{ style.get(sides) });
			}
		}

		template<meta::widget widget_type>
		void render(widget_type& widget)
		{
			if (presenter_ == nullptr) return;

			auto& buffer = presenter_->get_draw_buffer();
			render::context ctx{ buffer, {} };
			auto size = buffer.get_size();
			widget.arrange(rect(0, 0, size.width - 1, size.height - 1));
			widget.render(ctx);

			for (const auto& [type, boxes] : ctx.frames)
			{
				switch (type)
				{
					case frame::type::single_line: frames<frame::single_line>(boxes); break;
					case frame::type::double_line: frames<frame::double_line>(boxes); break;
					case frame::type::heavy_line: frames<frame::heavy_line>(boxes); break;
					case frame::type::rounded_line: frames<frame::rounded_line>(boxes); break;
					default: break;
				}
			}
		}

		template<meta::presenter<backend_type> presenter_type, typename... arguments>
		void set_presenter(arguments&&... args)
		{
			presenter_ = std::make_unique<presenter_type>(backend_, active_size_, std::forward<arguments>(args)...);
		}

		void set_present_mode(present_mode mode)
		{
			switch (mode)
			{
				case present_mode::immediate: set_presenter<immediate_presenter<backend_type>>(); break;
				case present_mode::double_buffered: break;
				default: break;
			}
		}

		draw_buffer& get_draw_buffer()
		{
			if (presenter_ == nullptr) throw std::runtime_error("presenter is not set.");
			return presenter_->get_draw_buffer();
		}

		const draw_buffer& get_draw_buffer() const
		{
			if (presenter_ == nullptr) throw std::runtime_error("presenter is not set.");
			return presenter_->get_draw_buffer();
		}

		size get_buffer_size() const
		{
			if (presenter_ == nullptr) return {};
			return presenter_->get_draw_buffer().get_size();
		}
	};
}
