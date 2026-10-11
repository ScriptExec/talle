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

		virtual void set(position pos, const cell& cell) = 0;

		virtual void on_resize(size new_size) = 0;
		virtual void on_clear(const std::optional<cell>& fill_cell = std::nullopt) = 0;
		virtual void on_clear(const rect& area, const std::optional<cell>& fill_cell = std::nullopt) = 0;
		virtual void on_render() = 0;
	};

	namespace meta
	{
		template<typename type, typename backend_type>
		concept presenter = std::derived_from<type, talle::presenter<backend_type>>;
	}
}
