#pragma once
#include <vector>
#include <optional>
#include <talle/render/presenter.hpp>
#include <talle/render/draw_buffer.hpp>
#include <talle/meta/writer.hpp>
#include <talle/cmd/set_cursor_pos.hpp>

namespace talle
{
	template<meta::backend backend_type>
	class buffered_presenter : public presenter<backend_type>
	{
	public:
		buffered_presenter(backend_type& backend, size initial_size) : presenter<backend_type>{ backend }
		{
			presenter<backend_type>::on_resize(initial_size);
		}
	private:
		std::optional<draw_buffer> last_buffer_;

	public:
		void render(const draw_buffer& buffer)
		{
			const auto& buffer_ = this->get_draw_buffer();
			auto& back = presenter<backend_type>::backend();
			back.flush();
			for (uint16_t y = 0; y < buffer_.height(); ++y)
			{
				back.template execute<cmd::set_cursor_pos>(position{ 0, y });
				for (uint16_t x = 0; x < buffer_.width(); ++x)
				{
					back.write(buffer_({ x, y }).data());
				}
			}
		}

		void render_with_diff(const draw_buffer& buffer, const draw_buffer_diff& diff)
		{
			auto& back = presenter<backend_type>::backend();
			for (auto range : diff.ranges)
			{
				auto y = range.start.y;
				back.template execute<cmd::set_cursor_pos>(position{ range.start.x, y });
				for (uint16_t x = range.start.x; x <= range.end.x; x++)
				{
					back.write(buffer({ x, y }).data());
				}
			}
		}

		virtual void on_resize(size new_size) override
		{
			presenter<backend_type>::on_resize(new_size);
			last_buffer_.reset();
		}

		virtual void on_render() override
		{
			const auto& buffer_ = this->get_draw_buffer();
			auto& back = presenter<backend_type>::backend();

			bool update_last_buf = true;
			if (!last_buffer_.has_value())
			{
				render(buffer_);
			}
			else
			{
				const auto& last_buf = last_buffer_.value();
				auto diff = last_buf.diff(buffer_);
				bool has_changes = diff.has_changes();
				if (has_changes)
				{
					render_with_diff(buffer_, diff);
				}
				update_last_buf = has_changes;
			}
			if (update_last_buf)
			{
				last_buffer_ = buffer_;
			}
		}
	};
}
