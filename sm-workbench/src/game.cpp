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
    sm::debug::printf("size of ptr: %i\n", sizeof(char*));
    sm::debug::printf("size of size_t: %i\n", sizeof(size_t));

    sm::memory::arena_t* app_arena = sm::memory::init_arena(KiB(1));

	sm::render::window_t* window = sm::render::init_window(app_arena, "SM Workbench", 1920, 1080, true);

    //sm::static_array_t<sm::window_msg_cb_with_args_t> m_msg_cbs = sm::static_array_init<sm::window_msg_cb_with_args_t>(app_arena, 4);

    //sm::window_msg_cb_with_args_t test_cb;
    //m_msg_cbs[0] = test_cb;

    sm::memory::free(app_arena, 50);

	return 0;
}