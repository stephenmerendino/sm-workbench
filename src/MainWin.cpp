#include "Engine/Platform/WindowsInclude.h"

#include <cstdio>
#include <sstream>

#include "App.h"
#include "Engine/Core/Common.h"

int WINAPI WinMain(HINSTANCE app, HINSTANCE prevApp, LPSTR args, int show)
{
	Unused(app);
	Unused(prevApp);
	Unused(args);
	Unused(show);

	App mainApp;
	mainApp.Run();

	return 0;
}
