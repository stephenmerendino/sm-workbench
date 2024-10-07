#include "game.h"

#include "sm/core/types.h"
#include "sm/core/assert.h"
#include "sm/memory/arena.h"

#define WINDOWS_MEAN_AND_LEAN
#include <windows.h>

// containers
namespace sm
{
    // static_array
    // dynamic_array
    // ring_buffer
    // queue
}

// string
namespace sm
{
}

// window
namespace sm
{
    typedef void (*window_msg_cb)(UINT msg, WPARAM w, LPARAM l, void* user_args);
    struct window_msg_cb_with_args_t 
    {
        window_msg_cb cb	= nullptr;
        void* args			= nullptr;
    };

    bool operator==(const window_msg_cb_with_args_t& a, const window_msg_cb_with_args_t& b)
    {
        return a.cb == b.cb && a.args == b.args;
    }

    struct window_t
    {
        HWND hwnd;
        const char* name;
        u32 width;
        u32 height;
        u32 x;
        u32 y;
        bool is_minimized;
        bool was_resized;
        bool was_closed;
        bool is_moving;
        //std::vector<WindowMsgCallbackWithArgs> m_msgCallbacks;
        byte_t padding[4];
    };

    window_t* init_window()
    {
        return nullptr;
    }
}

#include <cstdio>

int run_game()
{
	sm::window_t* window = sm::init_window();

    sm::memory_arena_t* app_arena = sm::arena_init(KiB(1));

    char* s = (char*)arena_alloc_zero(app_arena, 50);
    sprintf_s(s, 50, "%s\n", "Testing...");
    printf("%s\n", s);

    sm::arena_free(app_arena, 50);

	//static_array<window_msg_cb_with_args_t> foo = init_static_array(app_arena, 50);
	//dynamic_array<window_msg_cb_with_args_t> foo = init_dynamic_array(app_arena, 50);

	return 0;
}