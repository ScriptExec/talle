#include <talle/talle.hpp>
#include <talle/render/renderer.hpp>
#include <talle/render/frame/rounded_line.hpp>
#include <cmath>
#include <chrono>
#include <numbers>
#include <talle/render/widgets/layout.hpp>

using namespace talle;

int main(int argc, char* argv[])
{
	auto back = backend{ std::cout };
	auto term = terminal{ back };
	auto rnd = renderer{ back };

	term.set_cursor_visible(false);

	auto term_size_opt = term.size();
	if (!term_size_opt.has_value())
	{
		term.writeln("Failed to get terminal size.");
		return 1;
	}
	auto term_size = term_size_opt.value();
	rnd.resize(term_size);
	rnd.set_present_mode(present_mode::immediate);

	auto start_time = std::chrono::steady_clock::now();
	while (true)
	{
		term_size_opt = term.size();
		if (term_size_opt.has_value())
		{
			term_size = term_size_opt.value();
		}

		auto event = term.read_event();
		bool clear{};
		if (event.has_value())
		{
			if (auto event_resize = event->try_get<resize_event>())
			{
				rnd.resize(event_resize->size);
			}
			if (auto event_key = event->try_get<key_event>())
			{
				if (auto event_key_chr = event_key->key.try_get<key::chr>())
				{
					if (event_key_chr->c == 'q' and event_key->modifiers.has(talle::key_modifier::ctrl))
					{
						break;
					}
				}
			}
		}

		term.reset_cursor_pos();

		auto frame_type{ frame::type::rounded_line };
		auto root = layout{ direction::horizontal };
		{
			root.set_flex(flex::space_around);
			root.set_frame_type(frame_type);

			auto& left_child = root.add<layout>(direction::horizontal);
			{
				auto& left = left_child.item;
				left->set_frame_type(frame_type);
				left->set_flex(flex::space_around);
				left_child.constraints.fill_space();

				auto& left_left_child = left->add<layout>(direction::vertical);
				{
					auto& left_left = left_left_child.item;
					left_left->set_frame_type(frame_type);
					left_left->set_flex(flex::space_between);
					left_left->set_gap(1);
					left_left->set_padding(1);
					left_left_child.constraints.fill_space();
					auto& left_left_top_child = left_left->add<layout>(direction::horizontal);
					{
						auto& left_left_top = left_left_top_child.item;
						left_left_top->set_frame_type(frame_type);
						left_left_top_child.constraints.fill_space();
					}
					auto& left_left_bottom_child = left_left->add<layout>(direction::horizontal);
					{
						auto& left_left_bottom = left_left_bottom_child.item;
						left_left_bottom->set_frame_type(frame_type);
						left_left_bottom_child.constraints.fill_space();
					}
				}

				auto& left_right_child = left->add<layout>(direction::vertical);
				{
					auto& left_right = left_right_child.item;
					left_right->set_frame_type(frame_type);
					left_right->set_flex(flex::space_between);
					left_right_child.constraints.fill_space();
					
					auto& left_right_top_child = left_right->add<layout>(direction::horizontal);
					{
						auto& left_right_top = left_right_top_child.item;
						left_right_top->set_frame_type(frame_type);
						left_right_top_child.constraints.height_percentage = 1 / 2.6f;
						left_right_top_child.constraints.fill_space();
					}
					auto& left_right_middle_child = left_right->add<layout>(direction::horizontal);
					{
						auto& left_right_middle = left_right_middle_child.item;
						left_right_middle->set_frame_type(frame_type);
						left_right_middle_child.constraints.height_percentage = 1 / 4.f;
						left_right_middle_child.constraints.fill_space();
					}
					auto& left_right_bottom_child = left_right->add<layout>(direction::horizontal);
					{
						auto& left_right_bottom = left_right_bottom_child.item;
						left_right_bottom->set_frame_type(frame_type);
						left_right_bottom_child.constraints.height_percentage = 1 / 8.f;
						left_right_bottom_child.constraints.fill_space();
					}

				}
			}

			auto& right_child = root.add<layout>(direction::vertical);
			{
				auto& right = right_child.item;
				right->set_frame_type(frame_type);
				right->set_padding(1);
				right->set_flex(flex::space_between);
				right_child.constraints.fill_space();

				auto& top_right_child = right->add<layout>(direction::horizontal);
				{
					auto& top_right = top_right_child.item;
					top_right->set_frame_type(frame_type);
					top_right->set_padding(2);
					top_right_child.constraints.fill_space();
					{
						auto& top_right_left_child = top_right->add<layout>(direction::vertical);
						{
							auto& top_right_left = top_right_left_child.item;
							top_right_left->set_frame_type(frame_type);
							top_right_left_child.constraints.fill_space();
						}
						auto& top_right_right_child = top_right->add<layout>(direction::vertical);
						{
							auto& top_right_right = top_right_right_child.item;
							top_right_right->set_frame_type(frame_type);
							top_right_right_child.constraints.fill_space();
						}
					}
				}
				auto& bottom_right_child = right->add<layout>(direction::horizontal);
				{
					auto& bottom_right = bottom_right_child.item;
					bottom_right->set_frame_type(frame_type);
					bottom_right_child.constraints.fill_space();
				}
			}
		}		
		rnd.clear();
		rnd.render(root);
		rnd.render();
	}
	term.set_cursor_visible(true);
}
