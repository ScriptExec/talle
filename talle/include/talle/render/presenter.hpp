#pragma once
#include <optional>
#include <concepts>
#include <type_traits>

#include <talle/utils/size.hpp>
#include <talle/utils/rect.hpp>
#include <talle/utils/position.hpp>
#include <talle/core/backend.hpp>
#include <talle/render/draw_buffer.hpp>

#include "cell.hpp"

namespace talle
{
	template<meta::backend backend_type>
	class presenter
	{
	public:
		presenter(backend_type& backend) : backend_{ backend } {}

	protected:
		draw_buffer buffer_;
	private:
		backend_type& backend_;

	public:
		backend_type& backend()
		{
			return backend_;
		}

		const backend_type& backend() const
		{
			return backend_;
		}
		
		draw_buffer& get_draw_buffer()
		{
			return buffer_;
		}

		const draw_buffer& get_draw_buffer() const
		{
			return buffer_;
		}

		void render_cell(const draw_buffer& buffer, const position& pos, const rect& range)
		{
			auto& cell = buffer.at(pos);
			auto style = cell.style();
			bool is_first_cell = (pos.x == range.start.x and pos.y == range.start.y);
			bool is_last_cell = (pos.x == range.end.x and pos.y == range.end.y);
			bool reset_attributes = is_first_cell or is_last_cell;

			if (is_first_cell)
			{
				backend_.write(cmd::set_style{ style });
			}
			else
			{
				auto prev_style = buffer[pos.y * buffer.width() + pos.x - 1].style();
				backend_.write(cmd::update_style{ prev_style, style });
			}
			backend_.write(buffer.at(pos).data());
			if (reset_attributes)
			{
				backend_.write(cmd::reset_attributes{});
			}
		}

		virtual void set(position pos, const cell& cell)
		{
			auto size = buffer_.get_size();
			if (pos.x >= size.width or pos.y >= size.height) return;

			buffer_.set(pos, cell);
		}

		virtual void on_resize(size new_size)
		{
			buffer_.resize(new_size);
		}

		virtual void on_clear(const std::optional<cell>& fill_cell = std::nullopt)
		{
			buffer_.fill(fill_cell.value_or(cell::empty()));
		}

		virtual void on_clear(const rect& area, const std::optional<cell>& fill_cell = std::nullopt)
		{
			buffer_.fill(area, fill_cell.value_or(cell::empty()));
		}

		virtual void on_render() = 0;
	};

	namespace meta
	{
		template<typename type, typename backend_type>
		concept presenter = std::derived_from<type, talle::presenter<backend_type>>;
	}
}
