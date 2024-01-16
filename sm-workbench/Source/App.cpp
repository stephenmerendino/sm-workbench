#include "App.h"
#include "Engine/Config.h"
#include "Engine/Core/Macros.h"
#include "Engine/Platform/WindowsInclude.h"
#include "Engine/Render/Window.h"

App g_app;

static void AppWindowMsgHandler(UINT msg, WPARAM wParam, LPARAM lParam, void* userArgs)
{
	UNUSED(lParam);
	UNUSED(userArgs);

	switch (msg)
	{
	case WM_CLOSE:
	{
		g_app.m_bIsRunning = false;
		break;
	}

	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
		{
			g_app.m_bIsRunning = false;
		}
		break;
	}

	default: break;
	}
}

App::App()
	:m_bIsRunning(false)
{
}

void App::Run()
{
	m_bIsRunning = true;

	// TODO: Use SetProcessDpiAwarenessContext to make dpi awareness work per monitor
	SetProcessDPIAware(); // make sure window is created with scaling handled

	Window* appWindow = new Window();
	appWindow->Init("SM Workbench", WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_ALLOW_RESIZE);
	appWindow->AddMsgCallback(AppWindowMsgHandler);

	while (m_bIsRunning)
	{
		appWindow->Update();
		::Sleep(100);

		// stats
		F32 ds = stopwatch_get_elapsed_seconds(frame_stopwatch);
		//report_fps(app_window, ds);

		//// begin frame
		//stopwatch_start(frame_stopwatch);
		//input_begin_frame();
		//renderer_begin_frame();

		//////////////////////////
		//game_begin_frame();
		//////////////////////////

		//// update
		//window_update(app_window);
		//input_update();
		//camera_update(scene_camera, ds);
		//renderer_update(ds);

		//////////////////////////
		//game_update(ds);
		//game_render();
		//////////////////////////

		//// render
		//renderer_render_frame();

		//// end frame
		//f32 frame_time_secs = stopwatch_get_elapsed_seconds(frame_stopwatch);
		//sleep_remaining_frame(frame_time_secs);
	}
}