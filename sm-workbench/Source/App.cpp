#include "App.h"
#include "Engine/Config.h"
#include "Engine/Core/Assert.h"
#include "Engine/Core/Macros.h"
#include "Engine/Core/Time.h"
#include "Engine/Input/InputSystem.h"
#include "Engine/Job/JobSystem.h"
#include "Engine/Platform/WindowsInclude.h"
#include "Engine/Render/Window.h"
#include <sstream>

App g_app;

// TODO: Use the input system to check for escape key or close button press
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
	,m_pAppWindow(nullptr)
{
}

void App::Run()
{
	// TODO: Use SetProcessDpiAwarenessContext to make dpi awareness work per monitor
	// TODO: Move this to Window class?
	SetProcessDPIAware(); // make sure window is created with scaling handled

	m_pAppWindow = new Window();
	m_pAppWindow->Init("SM Workbench", WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_ALLOW_RESIZE);
	m_pAppWindow->AddMsgCallback(AppWindowMsgHandler);

	SM_ASSERT(Time::Init());
	SM_ASSERT(g_jobSystem.Init());
	SM_ASSERT(g_inputSystem.Init(m_pAppWindow));
	//SM_ASSERT(g_renderer.Init());

	Stopwatch frameStopwatch;
	F32 ds = 0.0f;
	frameStopwatch.Start();

	m_bIsRunning = true;
	while (m_bIsRunning)
	{
		// begin frame
		g_inputSystem.BeginFrame();
		//g_renderer.BeginFrame();

		//////////////////////////
		//game_begin_frame();
		//////////////////////////

		// update
		m_pAppWindow->Update();
		g_inputSystem.Update();
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

		// stats
		ds = frameStopwatch.GetElapsedSeconds();
		SleepRemainingFrame(ds);

		frameStopwatch.Start();

		ReportFps(ds);
	}

	//g_renderer.Shutdown();
	g_inputSystem.Shutdown();
	g_jobSystem.Shutdown();
	m_pAppWindow->Destroy();
}

void App::ReportFps(F32 ds)
{
	static I32 s_frameCount = 0;
	static F32 s_frameTimeAccrual = 0.0f;

	s_frameCount++;
	s_frameTimeAccrual += ds;

	if (s_frameTimeAccrual >= FPS_CALC_TIME_INTERVAL_SECONDS)
	{
		F32 avgCurrentFps = (F32)s_frameCount / s_frameTimeAccrual;

		std::ostringstream ss;
		ss << "sm workbench" << " - fps: " << avgCurrentFps	 << " - frame time ms: " << ds * 1000.0f;
		std::string newTitle = ss.str();
		m_pAppWindow->SetTitle(newTitle.c_str());

		s_frameCount = 0;
		s_frameTimeAccrual = 0.0f;
	}
}

void App::SleepRemainingFrame(F32 ds)
{
	const F32 targetSecondsPerFrame = (1.0f / TARGET_FPS);

	// frame took longer than our target time
	if (ds >= targetSecondsPerFrame)
	{
		return;
	}

	F32 timeToSleepSeconds = targetSecondsPerFrame - ds;
	Thread::SleepSeconds(timeToSleepSeconds);
}

//ui_log_init();
//
//window_t* app_window = window_create("SM Workbench", WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_ALLOW_RESIZE);
//window_add_msg_callback(app_window, window_message_handler);
//
//job_system_init();
//time_init();
//input_init(app_window);
//renderer_init(app_window);
//
//camera_t scene_camera;
//scene_camera.m_world_pos = make_vec3(0.0f, 0.0f, 0.0f);
//camera_look_at(scene_camera, VEC3_FORWARD);
//renderer_set_main_camera(&scene_camera);
//
//////////////////////////
//game_init();
//////////////////////////
//
//stopwatch_t frame_stopwatch;
//stopwatch_start(frame_stopwatch);
//
//s_is_running = true;
//while (s_is_running)
//{
//	// stats
//	f32 ds = stopwatch_get_elapsed_seconds(frame_stopwatch);
//	report_fps(app_window, ds);
//
//	// begin frame
//	stopwatch_start(frame_stopwatch);
//	input_begin_frame();
//	renderer_begin_frame();
//
//	////////////////////////
//	game_begin_frame();
//	////////////////////////
//
//	// update
//	window_update(app_window);
//	input_update();
//	camera_update(scene_camera, ds);
//	renderer_update(ds);
//
//	////////////////////////
//	game_update(ds);
//	game_render();
//	////////////////////////
//
//	// render
//	renderer_render_frame();
//
//	// end frame
//	f32 frame_time_secs = stopwatch_get_elapsed_seconds(frame_stopwatch);
//	sleep_remaining_frame(frame_time_secs);
//}