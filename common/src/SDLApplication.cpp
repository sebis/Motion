#include "Keymap.h"
#include "SDLApplication.h"
#include "Trace.h"

namespace Common
{
	SDLApplication::SDLApplication(bool fixedTimeStep, float targetElapsedTime)
		: m_totalTime(0),
		m_fixedTimeStep(fixedTimeStep),
		m_targetElapsedTime(targetElapsedTime)
	{
	}

	bool SDLApplication::init(int argc, char * argv[])
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
			Trace::error("Could not initialize SDL: %s\n", SDL_GetError());
			SDL_Quit();
			return false;
		}

		const SDL_VideoInfo * videoInfo = SDL_GetVideoInfo();
		if (videoInfo == NULL) {
			Trace::error("Could not get video information: %s\n", SDL_GetError());
			SDL_Quit();
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		m_surface = SDL_SetVideoMode(800, 600, videoInfo->vfmt->BitsPerPixel, SDL_OPENGL | SDL_RESIZABLE);
		if (!m_surface) {
			Trace::error("Could not set video mode: %s\n", SDL_GetError());
			SDL_Quit();
			return false;
		}
		window_resized(m_surface->w, m_surface->h);

		return true;
	}

	int SDLApplication::run()
	{
		bool quit = false;
		SDL_Event event;

		m_totalTime = SDL_GetTicks();

		while (!quit)
		{
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q)
						quit = true;
					else if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
						keyDown(KEY_MOVE_FORWARD);
					else if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
						keyDown(KEY_MOVE_BACKWARD);
					else if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
						keyDown(KEY_MOVE_LEFT);
					else if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
						keyDown(KEY_MOVE_RIGHT);
					break;
				case SDL_KEYUP:
					if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q)
						quit = true;
					else if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
						keyUp(KEY_MOVE_FORWARD);
					else if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
						keyUp(KEY_MOVE_BACKWARD);
					else if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
						keyUp(KEY_MOVE_LEFT);
					else if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
						keyUp(KEY_MOVE_RIGHT);
					else if (event.key.keysym.sym == SDLK_1)
						keyUp(KEY_RESET_1);
					else if (event.key.keysym.sym == SDLK_2)
						keyUp(KEY_RESET_2);
					break;
				case SDL_MOUSEMOTION:
					if (event.button.button == SDL_BUTTON_LEFT)
						mouse(KEY_MOUSE_LEFT, event.motion.xrel, event.motion.yrel);
					break;
				case SDL_VIDEORESIZE:
					{
						Uint8 bpp = m_surface->format->BitsPerPixel;
						Uint32 flags = m_surface->flags;
						SDL_FreeSurface(m_surface);
						m_surface = SDL_SetVideoMode(event.resize.w, event.resize.h, bpp, flags);
						window_resized(event.resize.w, event.resize.h);
					}
					break;
				case SDL_QUIT:
					quit = true;
					break;
				}
			}

			int currentTime = SDL_GetTicks();
			float dt = float(currentTime - m_totalTime);

			if (!m_fixedTimeStep || dt >= m_targetElapsedTime * 1000) {
				m_totalTime = currentTime;
				if (dt > 0)
					update(dt);
			}

			draw();

			SDL_GL_SwapBuffers();
		}

		SDL_Quit();

		return 0;
	}

	void SDLApplication::window_resized(int width, int height)
	{
	}

	void SDLApplication::update(float dt)
	{
	}

	void SDLApplication::draw()
	{
	}
}