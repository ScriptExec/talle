#pragma once
#include <vector>
#include <talle/render/presenter.hpp>
#include <talle/render/draw_buffer.hpp>
#include <talle/meta/writer.hpp>
#include <talle/cmd/set_cursor_pos.hpp>

namespace talle
{
	template<meta::backend backend_type>
	class immediate_presenter : public presenter<backend_type>
	{
	public:
		immediate_presenter(backend_type& backend, size initial_size) : presenter<backend_type>{ backend }
		{
			on_resize(initial_size);
		}

	public:
		virtual void set(position pos, const cell& cell) override
		{
			auto& buffer_ = this->get_draw_buffer();
			auto size = buffer_.get_size();
			if (pos.x >= size.width or pos.y >= size.height) return;

			buffer_.set(pos, cell);
		}

		virtual void on_resize(size new_size) override
		{
			auto& buffer_ = this->get_draw_buffer();
			buffer_.resize(new_size);
		}

		virtual void on_clear(const std::optional<cell>& fill_cell = std::nullopt) override
		{
			auto& buffer_ = this->get_draw_buffer();
			if (fill_cell.has_value())
			{
				buffer_.fill(fill_cell.value());
			}
			else
			{
				buffer_.fill(cell{});
			}
		}

		virtual void on_clear(const rect& area, const std::optional<cell>& fill_cell = std::nullopt) override
		{
			auto& buffer_ = this->get_draw_buffer();
			for (uint16_t y = area.start.y; y <= area.end.y; ++y)
			{
				for (uint16_t x = area.start.x; x <= area.end.x; ++x)
				{
					if (fill_cell.has_value())
					{
						buffer_.set({ x, y }, fill_cell.value());
					}
					else
					{
						buffer_.set({ x, y }, cell{});
					}
				}
			}
		}

		virtual void on_render() override
		{
			const auto& buffer_ = this->get_draw_buffer();
			auto& back = presenter<backend_type>::backend();
			for (uint16_t y = 0; y < buffer_.height(); ++y)
			{
				back.template execute<cmd::set_cursor_pos>(position{ 0, y });
				for (uint16_t x = 0; x < buffer_.width(); ++x)
				{
					back.write(buffer_({ x, y }).data());
				}
			}
		}
	};
}
