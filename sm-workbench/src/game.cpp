#include "game.h"

#define WINDOWS_MEAN_AND_LEAN
#include <windows.h>

#include <cstdint>

typedef		uint8_t		u8;
typedef		uint16_t	u16;
typedef		uint32_t	u32;
typedef		uint64_t	u64;

typedef		int8_t		i8;
typedef		int16_t		i16;
typedef		int32_t		i32;
typedef		int64_t		i64;

typedef		u8			byte_t;

typedef void (*window_msg_cb)(UINT msg, WPARAM w, LPARAM l, void* user_args);
struct window_msg_cb_with_args_t 
{
	window_msg_cb cb;
	void* args;

	bool operator==(const window_msg_cb_with_args_t& cb_with_args)
	{
		return cb == cb_with_args.cb && args == cb_with_args.args;
	}
};

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

int run_game()
{
	window_t* window = init_window();

	static_array<window_msg_cb_with_args_t> foo = init_static_array(50);

	dynamic_array<window_msg_cb_with_args_t> foo = init_dynamic_array(50);

	return 0;
}