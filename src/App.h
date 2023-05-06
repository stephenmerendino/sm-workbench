#pragma once

#include "engine/core/Clock.h"
#include "engine/core/Types.h"
#include "engine/render/Camera.h"

#include <vector>
#include <set>
#include <string>

class Window;
class VulkanRenderer;

class App
{
public:
	App();
	~App();

	void Run();
	void SetIsRunning(bool bIsRunning) { m_bIsRunning = bIsRunning; }

private:
	void Setup();
	void SetupEngine();
	void SetupCamera();
	void Teardown();

	void MainLoop();
	void Update(float ds);
	void SleepFrame(F32 deltaSeconds);
	void ReportFps(F32 deltaSeconds);

private:
	bool m_bIsRunning;
	
	Window* m_pWindow;
	VulkanRenderer* m_pRenderer;
	Camera m_sceneCamera;

	size_t m_currentFrame;
	Clock m_frameClock;
	I32 m_frameCount;
	F32 m_frameTimeAccrual;
	I32 m_currentFramesPerSecond;
};
