#pragma once

#include "Engine/Core/Types.h"

class Window;

class App
{
public:
	App();
	void Run();

	void ReportFps(F32 ds);
	void SleepRemainingFrame(F32 ds);

	bool m_bIsRunning;
	Window* m_pAppWindow;
};

extern App g_app;
