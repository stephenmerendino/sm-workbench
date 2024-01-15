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
	SetProcessDPIAware(); // make sure window is created with scaling handled

	Window* appWindow = new Window();
	appWindow->Init("SM Workbench", WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_ALLOW_RESIZE);
	appWindow->AddMsgCallback(AppWindowMsgHandler);

	m_bIsRunning = true;
	while (m_bIsRunning)
	{
		appWindow->Update();
		::Sleep(100);
	}
}