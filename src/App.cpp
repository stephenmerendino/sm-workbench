#include "App.h"

#include "engine/core/Config.h"
#include "engine/core/Common.h"
#include "engine/core/FileUtils.h"
#include "engine/input/InputSystem.h"
#include "engine/math/MathUtils.h"
#include "engine/render/vulkan/VulkanInstance.h"
#include "engine/render/vulkan/VulkanRenderer.h"
#include "engine/render/vulkan/VulkanRenderer2.h"
#include "engine/render/Window.h"
#include "engine/thread/Thread.h"

#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <array>

static void AppWindowMessageHandler(UINT msg, WPARAM wParam, LPARAM lParam, void* userArgs)
{
	Unused(lParam);
	App* pApp = (App*)userArgs;

	switch (msg)
	{
		case WM_CLOSE:
		{
			pApp->SetIsRunning(false);
			break;
		}

		case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE)
			{
				pApp->SetIsRunning(false);
			}
			break;
		}

		default: break;
	}
}

App::App()
	:m_bIsRunning(false)
	,m_pWindow(nullptr)
	,m_pRenderer(nullptr)
	,m_currentFrame(0)
	,m_frameCount(0)
	,m_frameTimeAccrual(0.0f)
	,m_currentFramesPerSecond(0)
{
}

App::~App()
{
}

void App::Run()
{
	Setup();
	MainLoop();
	Teardown();
}

void App::Setup()
{
    // Setup window
	m_pWindow = new Window("SM Workbench", WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_ALLOW_RESIZE);
	m_pWindow->Setup();
	m_pWindow->AddMessageCallback(AppWindowMessageHandler, this);

    // Setup engine
	Clock::Init();
	g_inputSystem.Init(m_pWindow);
	m_pRenderer = new VulkanRenderer(m_pWindow);
	m_pRenderer->Setup();

    // Setup camera
	m_sceneCamera.SetPosition(Vec3(5.0f, 5.0f, 5.0f));
	m_sceneCamera.LookAt(Vec3::ZERO, Vec3::UP);
	m_pRenderer->SetCamera(&m_sceneCamera);

	m_bIsRunning = true;
}

void App::MainLoop()
{
	while (m_bIsRunning)
	{
		F32 deltaSeconds = m_frameClock.GetSecondsElapsed();
		m_frameClock.Start();

		Update(deltaSeconds);

		// No need to render if window is minimized
		if (!m_pWindow->IsMinimized())
		{
			m_pRenderer->RenderFrame();
		}

		ReportFps(deltaSeconds);
		
		F32 timeToRunFrame = m_frameClock.GetSecondsElapsed();
		SleepFrame(timeToRunFrame);
	}
}

void App::ReportFps(F32 deltaSeconds)
{
	m_frameCount++;
	m_frameTimeAccrual += deltaSeconds;

	if (m_frameTimeAccrual >= FPS_CALC_TIME_INTERVAL_SECONDS)
	{
		m_currentFramesPerSecond = static_cast<I32>(static_cast<F32>(m_frameCount) / m_frameTimeAccrual);

		std::ostringstream stringStream;
		stringStream << "SM Workbench" << " - Current FPS: " << m_currentFramesPerSecond << "\n";
		std::string newWindowTitle = stringStream.str();
		m_pWindow->SetTitle(newWindowTitle);

		m_frameCount = 0;
		m_frameTimeAccrual = 0.0f;
	}
}

void App::Update(float ds)
{
	g_inputSystem.BeforeWindowUpdate();
	m_pWindow->Update();
	g_inputSystem.Update();
	m_sceneCamera.Update(ds);
}

void App::SleepFrame(F32 deltaSeconds)
{
	const F32 targetSecondsPerFrame = 1.0f / TARGET_FPS;

	// frame took longer than our target time
	if (deltaSeconds >= targetSecondsPerFrame)
	{
		return;
	}

	F32 timeToSleepMs = (targetSecondsPerFrame - deltaSeconds) * 1000.0f;
	Thread::PreciseSleepMs(timeToSleepMs);
}

void App::Teardown()
{
	m_pRenderer->Teardown();
	delete m_pRenderer;

	m_pWindow->Teardown();
	delete m_pWindow;
}
