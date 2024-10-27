#include "game.h"

#include "sm/core/static_array.h"
#include "sm/core/assert.h"
#include "sm/core/debug.h"
#include "sm/core/types.h"
#include "sm/memory/arena.h"

#define WINDOWS_MEAN_AND_LEAN
#include <windows.h>

#include <cstdio>

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
        sm::static_array<window_msg_cb> m_msg_cbs;
        byte_t padding[4];
    };

    window_t* init_window()
    {
        return nullptr;
    }
}

// string
namespace sm
{
    struct static_string
    {
        static_array<char> c_str;

        char& operator[](size_t index)
        {
            SM_ASSERT(index <= c_str.size);
            return c_str[index];
        }

        const char& operator[](size_t index) const
        {
            SM_ASSERT(index <= c_str.size);
            return c_str[index];
        }

        void operator=(const char* str)
        {
            size_t len = strlen(str);
            sm::static_array_copy(c_str, str, len + 1);
            c_str[len] = '\0';
        }

        void operator=(static_string str)
        {
            *this = str.c_str.data;
        }
    };

    static_string static_string_init(memory_arena_t* arena, size_t size)
    {
        static_string str;
        str.c_str = static_array_init<char>(arena, size);
        return str;
    }
}

#include <vector>
#include <string>

int run_game()
{
    sm::debug_printf("size of ptr: %i\n", sizeof(char*));
    sm::debug_printf("size of size_t: %i\n", sizeof(size_t));
    sm::debug_printf("size of std::vector: %i\n", sizeof(std::vector<char>));
    sm::debug_printf("size of std::string: %i\n", sizeof(std::string));

	sm::window_t* window = sm::init_window();

    sm::memory_arena_t* app_arena = sm::arena_init(KiB(1));

    char* s = (char*)arena_alloc_zero(app_arena, 50);
    sprintf_s(s, 50, "%s", "Hello World");
    sm::debug_printf("%s\n", s);

    sm::static_array<int> nums = sm::static_array_init<int>(app_arena, 50);
    nums[12] = 4;
    int test = nums[12];
    sm::debug_printf("%i\n", test);

    sm::static_string test_str = sm::static_string_init(app_arena, 50);
    test_str = "Testing out my static_string class";
    sm::debug_printf("%s\n", test_str.c_str.data);

    sm::static_string test_str2 = sm::static_string_init(app_arena, 30);
    test_str2 = "Foo bar";
    sm::debug_printf("%s\n", test_str2.c_str.data);

    test_str = test_str2;
    sm::debug_printf("%s\n", test_str.c_str.data);

    sm::arena_free(app_arena, 50);

	//static_array<window_msg_cb_with_args_t> foo = init_static_array(app_arena, 50);
	//dynamic_array<window_msg_cb_with_args_t> foo = init_dynamic_array(app_arena, 50);

	return 0;
}