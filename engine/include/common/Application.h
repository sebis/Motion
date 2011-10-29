#ifndef COMMON_APPLICATION
#define COMMON_APPLICATION

#include "Keymap.h"

namespace Common
{
	class Application
	{
	public:
		Application() {};
		virtual ~Application() {};

		virtual bool init(int argc, char * argv[]) = 0;
		virtual int run() = 0;

		virtual void keyDown(Key /*key*/) {}
		virtual void keyUp(Key /*key*/) {}
		virtual void mouse(Key /*key*/, int /*x*/, int /*y*/) {}

		virtual void window_resized(int width, int height) = 0;

		virtual void display_text(const char * text, int x, int y) = 0;

		virtual void update(float dt) = 0;
		virtual void draw() = 0;

	protected:
		int m_width;
		int m_height;
	};
}

#endif /* COMMON_APPLICATION */
