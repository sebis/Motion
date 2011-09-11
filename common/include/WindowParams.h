#ifndef COMMON_WINDOWPARAMS
#define COMMON_WINDOWPARAMS

struct WindowParams
{
	int x;
	int y;
	int width;
	int height;
};

static const WindowParams Default = { 100, 100, 800, 600 };

#endif