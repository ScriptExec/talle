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
			presenter<backend_type>::on_resize(initial_size);
		}

	public:

		virtual void on_render() override
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
	};
}
