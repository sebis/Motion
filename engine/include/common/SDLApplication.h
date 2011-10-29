#ifndef COMMON_SDLAPPLICATION
#define COMMON_SDLAPPLICATION

#include "Application.h"
#include "Keymap.h"
#include <SDL/SDL.h>

namespace Common
{
	class SDLApplication : public Application
	{
	public:
		SDLApplication(bool fixedTimeStep = true, float targetElapsedTime = 1.0f/60.0f);
		virtual ~SDLApplication() {}

		virtual bool init(int argc, char * argv[]);
		int run();

		virtual void keyDown(Key key) {}
		virtual void keyUp(Key key) {}

		virtual void window_resized(int width, int height);

		virtual void display_text(const char * text, int x, int y);

		virtual void update(float dt);
		virtual void draw();

	private:
		SDL_Surface * m_surface;
		bool m_fixedTimeStep;
		float m_targetElapsedTime;
		int m_totalTime;
	};
}

#endif