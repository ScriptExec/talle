#pragma once
#include <talle/meta/writer.hpp>

namespace talle::style
{
	enum class attribute
	{
		reset,
		bold,
		dim,
		italic,
		underlined,
		underlined_double,
		underlined_curled,
		underlined_dotted,
		underlined_dashed,
		slow_blink,
		rapid_blink,
		invert,
		hidden,
		strikethrough,
		framed,
		encircled,
		overlined,
		normal_intensity,
		no_bold,
		no_italic,
		no_underline,
		no_blink,
		no_invert,
		no_hidden,
		no_strikethrough,
		no_frame_or_encircle,
		no_overline,
	};

	template<talle::meta::output_writer writer_type>
	writer_type& operator<<(writer_type& writer, const attribute& attr);
}
