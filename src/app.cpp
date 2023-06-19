#include "app.h"
#include "engine/job/job_system.h"
#include "engine/render/Camera.h"
#include "game.h"
#include "engine/core/debug.h"
#include "engine/core/config.h"
#include "engine/core/macros.h"
#include "engine/core/time.h"
#include "engine/input/input.h"
#include "engine/platform/windows_include.h"
#include "engine/render/camera.h"
#include "engine/render/vulkan/vulkan_renderer.h"
#include "engine/render/window.h"
#include "engine/thread/thread.h"

#include <sstream>

static bool s_is_running = false;

static 
void window_message_handler(UINT msg, WPARAM w_param, LPARAM l_param, void* user_args)
{
    UNUSED(l_param);
    UNUSED(user_args);

    switch (msg)
    {
        case WM_CLOSE:
            {
                s_is_running = false;
                break;
            }

        case WM_KEYDOWN:
            {
                if (w_param == VK_ESCAPE)
                {
                    s_is_running = false;
                }
                break;
            }

        default: break;
    }
}

static
void report_fps(window_t* window, f32 ds)
{
    static i32 s_frame_count = 0;
    static f32 s_frame_time_accrucal = 0.0f;

	s_frame_count++;
	s_frame_time_accrucal += ds;

	if (s_frame_time_accrucal >= FPS_CALC_TIME_INTERVAL_SECONDS)
	{
		f32 avg_current_fps = (f32)s_frame_count / s_frame_time_accrucal;

		std::ostringstream ss;
		ss << "sm workbench" << " - fps: " << avg_current_fps << " - frame time ms: " << ds * 1000.0f;
		std::string new_title = ss.str();
        window_set_title(window, new_title.c_str());

		s_frame_count = 0;
		s_frame_time_accrucal = 0.0f;
	}
}

static
void sleep_remaining_frame(f32 frame_time_seconds)
{
	const f32 target_seconds_per_frame = 1.0f / TARGET_FPS;

	// frame took longer than our target time
	if (frame_time_seconds >= target_seconds_per_frame)
	{
		return;
	}

	f32 time_to_sleep_ms = (target_seconds_per_frame - frame_time_seconds) * 1000.0f;
    thread_sleep_ms(time_to_sleep_ms);
}

void run_app()
{
    window_t* app_window = window_create("SM Workbench", WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_ALLOW_RESIZE);
    window_add_msg_callback(app_window, window_message_handler);

    job_system_init();
    time_init();
    input_init(app_window);
    renderer_init(app_window);

    camera_t scene_camera;
    scene_camera.m_world_pos = make_vec3(0.0f, 0.0f, 0.0f);
    camera_look_at(scene_camera, VEC3_FORWARD);
    renderer_set_main_camera(&scene_camera);

    ////////////////////////
    game_init();
    ////////////////////////

    stopwatch_t frame_stopwatch;
    stopwatch_start(frame_stopwatch);

    s_is_running = true;
    while(s_is_running)
    {
        // stats
        f32 ds = stopwatch_get_elapsed_seconds(frame_stopwatch);
        report_fps(app_window, ds);

        // begin frame
        stopwatch_start(frame_stopwatch);
        input_begin_frame();

        ////////////////////////
        game_begin_frame();
        ////////////////////////
        
        // update
        window_update(app_window);
        input_update();
        camera_update(scene_camera, ds);
        renderer_update(ds);

        ////////////////////////
        game_update(ds);
        game_render();
        ////////////////////////

        // render
        renderer_render_frame();
        
        // end frame
        f32 frame_time_secs = stopwatch_get_elapsed_seconds(frame_stopwatch);
        sleep_remaining_frame(frame_time_secs);
    }

    renderer_deinit();
    job_system_shutdown();
    window_destroy(app_window);
}
