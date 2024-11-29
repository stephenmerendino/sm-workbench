#include "game.h"

#include "sm/config.h"
#include "sm/core/array.h"
#include "sm/core/string.h"
#include "sm/core/assert.h"
#include "sm/core/debug.h"
#include "sm/core/time.h"
#include "sm/core/types.h"
#include "sm/memory/arena.h"
#include "sm/render/window.h"

static bool s_is_running = true;

static void report_fps(sm::f32 ds)
{
	static sm::i32 s_frame_count = 0;
	static sm::f32 s_frame_time_accrual = 0.0f;

	s_frame_count++;
	s_frame_time_accrual += ds;

	if (s_frame_time_accrual >= FPS_CALC_TIME_INTERVAL_SECONDS)
	{
		sm::f32 avg_current_fps = (sm::f32)s_frame_count / s_frame_time_accrual;

	//	std::ostringstream ss;
	//	ss << "SM Workbench" << " - FPS: " << avg_current_fps	 << " - Frame Time(ms): " << ds * 1000.0f;
	//	std::string newTitle = ss.str();

		char new_title[256];
		//sprintf_s()

		sm::set_title(nullptr, new_title);

		s_frame_count = 0;
		s_frame_time_accrual = 0.0f;
	}
}

static void sleep_remaining_frame(sm::f32 ds)
{
	//const sm::f32 maxSecondsPerFrame = (1.0f / MAX_FPS);

	//// frame took longer than our max time
	//if (ds >= maxSecondsPerFrame)
	//{
	//	return;
	//}

	//sm::f32 timeToSleepSeconds = maxSecondsPerFrame - ds;
	//Thread::SleepSeconds(timeToSleepSeconds);
}

int run_game()
{
    sm::arena_t* app_startup_arena = sm::init_arena(KiB(1));

    sm::window_t* window = sm::init_window(app_startup_arena, "SM Workbench", 1920, 1080, true);
    sm::set_title(window, "Test");

    sm::init_time();

    sm::stopwatch_t frame_stopwatch;

	sm::f32 ds = 0.0f;
	while (s_is_running)
	{
        sm::update_window(window);

		// End Frame
		sm::f32 workTimeSeconds = sm::get_elapsed_seconds(&frame_stopwatch);
		sleep_remaining_frame(workTimeSeconds);

		// Set up next frame
		ds = get_elapsed_seconds(&frame_stopwatch);
		sm::start_stopwatch(&frame_stopwatch);
		report_fps(ds);
	}

	return 0;
}