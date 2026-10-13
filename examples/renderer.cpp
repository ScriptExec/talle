#include <talle/talle.hpp>
#include <talle/render/renderer.hpp>
#include <talle/render/frame/rounded_line.hpp>
#include <cmath>
#include <chrono>
#include <format>
#include <numbers>
#include <array>
#include <random>

using namespace talle;

static constexpr std::string_view charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+-=[]{}|;':\",./<>?`~";
static std::default_random_engine random{ std::random_device{}() };
static std::uniform_int_distribution<uint16_t> charset_distr{ 0, charset.size() - 1 };

static position random_pos(const rect& bounds)
{
	auto min_x = std::min(bounds.start.x, bounds.end.x);
	auto max_x = std::max(bounds.start.x, bounds.end.x);
	auto min_y = std::min(bounds.start.y, bounds.end.y);
	auto max_y = std::max(bounds.start.y, bounds.end.y);
	std::uniform_int_distribution<uint16_t> x_range{ min_x, max_x };
	std::uniform_int_distribution<uint16_t> y_range{ min_y, max_y };
	return { x_range(random), y_range(random) };
}

int main(int argc, char* argv[])
{
	constexpr bool show_cursor = true;

	auto back = backend{ std::cout };
	auto term = terminal{ back };
	auto rnd = renderer{ back };

	if (!show_cursor)
	{
		term.set_cursor_visible(false);
	}

	auto term_size_opt = term.size();
	if (!term_size_opt.has_value())
	{
		term.writeln("Failed to get terminal size.");
		return 1;
	}
	auto term_size = term_size_opt.value();
	rnd.resize(term_size);
	rnd.set_present_mode(present_mode::buffered);

	auto start_time = std::chrono::steady_clock::now();
	while (true)
	{
		auto frame_start_time = std::chrono::steady_clock::now();

		term_size_opt = term.size();
		if (term_size_opt.has_value())
		{
			term_size = term_size_opt.value();
		}

		bool clear{};
		auto event = term.read_event();
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

					if (event_key_chr->c == ' ' and event_key->is_press())
					{
						clear = true;
					}
				}
			}
		}

		if (clear)
		{
			rnd.clear();
			clear = false;
		}
		term.reset_cursor_pos();
		
		auto buf_size = rnd.get_buffer_size();
		auto draw_rect = rect(0, 0, buf_size.width - 1, buf_size.height - 1);
		rnd.render(random_pos(draw_rect), cell{ utf8_data{ charset.at(charset_distr(random))} });
		rnd.render();

		auto fps = 1.0 / std::chrono::duration<double>(std::chrono::steady_clock::now() - frame_start_time).count();
		term.set_title(std::format("FPS: {:.2f}", fps));
	}
	if (!show_cursor)
	{
		term.set_cursor_visible(true);
	}
}
