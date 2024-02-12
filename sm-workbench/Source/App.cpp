#include "App.h"
#include "Engine/Config.h"
#include "Engine/Core/Assert.h"
#include "Engine/Core/Debug.h"
#include "Engine/Core/Macros.h"
#include "Engine/Core/Time.h"
#include "Engine/Input/InputSystem.h"
#include "Engine/Job/JobSystem.h"
#include "Engine/Platform/WindowsInclude.h"
#include "Engine/Render/Camera.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Render/Window.h"
#include "Engine/Render/UI/UI.h"
#include <sstream>

App g_app;

// TODO: Use the input system to check for escape key or close button press
static void AppWindowMsgHandler(UINT msg, WPARAM wParam, LPARAM lParam, void* userArgs)
{
	UNUSED(lParam);
	UNUSED(wParam);
	UNUSED(userArgs);

	switch (msg)
	{
		case WM_CLOSE:
		{
			g_app.m_bIsRunning = false;
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

	UI::Init();

	m_pAppWindow = new Window();
	m_pAppWindow->Init("SM Workbench", WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_ALLOW_RESIZE);
	m_pAppWindow->AddMsgCallback(AppWindowMsgHandler);

	Time::Init();
	g_jobSystem.Init();
	g_inputSystem.Init(m_pAppWindow);
	g_renderer->Init(m_pAppWindow);

	Camera sceneCamera;
	sceneCamera.m_worldPos = Vec3::ZERO;
	sceneCamera.LookAt(Vec3::FORWARD);
	g_renderer->SetCamera(&sceneCamera);

	Stopwatch frameStopwatch;
	frameStopwatch.Start();
	F32 ds = 0.0f;

	m_bIsRunning = true;
	while (m_bIsRunning)
	{
		// Begin frame
		g_inputSystem.BeginFrame();

		// Update
		m_pAppWindow->Update();
		g_inputSystem.Update();
		sceneCamera.Update(ds);
		g_renderer->Update(ds);

		// Render
		g_renderer->Render();

		// End Frame
		F32 workTimeSeconds = frameStopwatch.GetElapsedSeconds();
		SleepRemainingFrame(workTimeSeconds);

		// Set up next frame
		ds = frameStopwatch.GetElapsedSeconds();
		frameStopwatch.Start();
		ReportFps(ds);

		// Check for escape key exit
		if (g_inputSystem.WasKeyPressed(KeyCode::KEY_ESCAPE))
		{
			m_bIsRunning = false;
		}
	}

	g_renderer->Shutdown();
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
		ss << "SM Workbench" << " - FPS: " << avgCurrentFps	 << " - Frame Time(ms): " << ds * 1000.0f;
		std::string newTitle = ss.str();
		m_pAppWindow->SetTitle(newTitle.c_str());

		s_frameCount = 0;
		s_frameTimeAccrual = 0.0f;
	}
}

void App::SleepRemainingFrame(F32 ds)
{
	const F32 maxSecondsPerFrame = (1.0f / MAX_FPS);

	// frame took longer than our max time
	if (ds >= maxSecondsPerFrame)
	{
		return;
	}

	F32 timeToSleepSeconds = maxSecondsPerFrame - ds;
	Thread::SleepSeconds(timeToSleepSeconds);
}
