#include "app.h"
#include "engine/core/debug.h"
#include "engine/core/config.h"
#include "engine/core/macros.h"
#include "engine/core/time.h"
#include "engine/render/window.h"
#include "engine/platform/windows_include.h"
#include "engine/input/input.h"

static bool s_is_running = false;

static void window_message_handler(UINT msg, WPARAM w_param, LPARAM l_param, void* user_args)
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

void run_app()
{
    window_t* app_window = create_window("SM Workbench", WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_ALLOW_RESIZE);
    add_window_callback(app_window, window_message_handler);

    init_time();
    init_input(app_window);
    //init_renderer();

    //camera_t scene_camera = create_camera(...);
    //set_render_camera(scene_camera);

    //stopwatch_t frame_stopwatch = create_stopwatch();
    //s_is_running = true;
    //while(s_is_running)
    //{
    //    // begin frame
    //    f32 ds = get_elapsed_time_seconds(frame_stopwatch);
    //    start_stopwatch(frame_stopwatch);
    //    input_system_begin_frame();

    //    // update
    //    update_window(*app_window);
    //    update_input_system();
    //    update_camera(scene_camera, ds);

    //    // render
    //    render_frame();
    //    
    //    // end frame
    //    report_fps(frame_stopwatch);
    //    sleep_remaining_frame(frame_stopwatch);
    //}

    //// cleanup
    //deinit_renderer();
    //destroy_window(app_window);
}
