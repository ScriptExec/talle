#pragma once

namespace term
{
	enum class clear_type
	{
		//all cells
		all,
		//all cells + history buffer
		all_and_history,
		//all cells from cursor down
		from_cursor_down,
		//all cells from cursor up
		from_cursor_up,
		//all cells from cursor to start of the line
		from_cursor_to_start,
		//all cells in cursor's line
		current_line,
		//all cells from cursor until newline
		until_newline,
	};
}
