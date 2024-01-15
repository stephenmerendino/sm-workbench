#pragma once

class App
{
public:
	App();
	void Run();

	bool m_bIsRunning;
};

extern App g_app;