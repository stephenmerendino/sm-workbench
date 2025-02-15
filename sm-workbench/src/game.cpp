#include "game.h"

#include "sm/config.h"
#include "sm/core/array.h"
#include "sm/core/string.h"
#include "sm/core/assert.h"
#include "sm/core/debug.h"
#include "sm/core/random.h"
#include "sm/core/time.h"
#include "sm/core/types.h"
#include "sm/io/device_input.h"
#include "sm/memory/arena.h"
#include "sm/render/window.h"
#include "sm/render/vk_renderer.h"
#include "sm/thread/thread.h"

static bool s_is_running = true;
static sm::window_t* s_app_window = nullptr;

static void report_fps(sm::f32 ds)
{
	static sm::i32 s_frame_count = 0;
	static sm::f32 s_frame_time_accrual = 0.0f;

	s_frame_count++;
	s_frame_time_accrual += ds;

	if (s_frame_time_accrual >= FPS_CALC_TIME_INTERVAL_SECONDS)
	{
		sm::f32 avg_current_fps = (sm::f32)s_frame_count / s_frame_time_accrual;

		char new_title[256];
		::sprintf_s(new_title, "SM Workbench - FPS: %f - Frame Time (ms): %f\n", avg_current_fps, ds * 1000.0f);

		sm::set_title(s_app_window, new_title);

		s_frame_count = 0;
		s_frame_time_accrual = 0.0f;
	}
}

static void sleep_remaining_frame(sm::f32 ds)
{
	const sm::f32 max_seconds_per_frame = (1.0f / MAX_FPS);

	// frame took longer than our max time
	if (ds >= max_seconds_per_frame)
	{
		return;
	}

	sm::f32 time_to_sleep_seconds = max_seconds_per_frame - ds;
	sm::sleep_thread_seconds(time_to_sleep_seconds);
}

static void game_window_msg_handler(sm::window_msg_type_t msg_type, sm::u64 msg_data, void* user_args)
{
	if(msg_type == sm::window_msg_type_t::CLOSE_WINDOW)
	{
		s_is_running = false;
	}
}

void test_stuff()
{
}

int run_game()
{
    sm::arena_t* app_startup_arena = sm::init_arena(KiB(1));

    s_app_window = sm::init_window(app_startup_arena, "SM Workbench", 1920, 1080, true);
    sm::set_title(s_app_window, "Test");
	sm::add_window_msg_cb(s_app_window, game_window_msg_handler, nullptr);

    sm::init_time();
	sm::init_random();
	sm::init_device_inputs(s_app_window);
	sm::init_renderer(s_app_window);

    sm::stopwatch_t frame_stopwatch;

	test_stuff();

	sm::f32 ds = 0.0f;
	while (s_is_running)
	{
        sm::update_window(s_app_window);

		// End Frame
		sm::f32 work_time_seconds = sm::get_elapsed_seconds(&frame_stopwatch);
		sleep_remaining_frame(work_time_seconds);

		// Set up next frame
		ds = get_elapsed_seconds(&frame_stopwatch);
		sm::start_stopwatch(&frame_stopwatch);
		report_fps(ds);

		if(sm::was_key_pressed(sm::key_code_t::KEY_ESCAPE))
		{
			s_is_running = false;
		}
	}

	return 0;
}