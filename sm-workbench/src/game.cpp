#include "game.h"

#include "sm/core/array.h"
#include "sm/core/string.h"
#include "sm/core/assert.h"
#include "sm/core/debug.h"
#include "sm/core/types.h"
#include "sm/memory/arena.h"
#include "sm/render/window.h"

int run_game()
{
    sm::debug_printf("size of ptr: %i\n", sizeof(char*));
    sm::debug_printf("size of size_t: %i\n", sizeof(size_t));

    sm::arena_t* app_arena = sm::init_arena(KiB(1));

    sm::window_t* window = sm::init_window(app_arena, "SM Workbench", 1920, 1080, true);

    size_t size_of_size = sizeof(size_t);

    sm::free(app_arena, 50);

	return 0;
}