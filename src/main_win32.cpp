#include "engine/platform/windows_include.h"
#include "engine/core/macros.h"
#include "app.h"

int WINAPI WinMain(HINSTANCE app, HINSTANCE prevApp, LPSTR args, int show)
{
	UNUSED(app);
	UNUSED(prevApp);
	UNUSED(args);
	UNUSED(show);

    run_app();

	return 0;
}
