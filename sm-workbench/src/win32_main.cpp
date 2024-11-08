#include "sm/platform/win32/win32_include.h"

#include "Engine/Core/Macros.h"

#include "game.h"

int WINAPI WinMain(_In_		HINSTANCE app, 
				   _In_opt_ HINSTANCE prevApp, 
				   _In_		LPSTR args, 
				   _In_		int show)
{
	UNUSED(app);
	UNUSED(prevApp);
	UNUSED(args);
	UNUSED(show);

	run_game();

	return 0;
}
