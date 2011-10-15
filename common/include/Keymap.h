#ifndef COMMON_KEYSYMS
#define COMMON_KEYSYMS

namespace Common
{
	typedef enum {
		KEY_MOVE_FORWARD = 100,
		KEY_MOVE_BACKWARD = 101,
		KEY_MOVE_LEFT = 102,
		KEY_MOVE_RIGHT = 103,
		KEY_RESET_1 = 104,
		KEY_RESET_2 = 105,
		KEY_CONTINUE = 106,

		KEY_MOUSE_NONE = 200,
		KEY_MOUSE_LEFT = 201,
		KEY_MOUSE_RIGHT = 202,
	} Key;
}

#endif /* COMMON_KEySYMS */