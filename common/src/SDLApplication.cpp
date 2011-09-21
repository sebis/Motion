#include "SDLApplication.h"
#include "Trace.h"

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

	if (!SDL_SetVideoMode(800, 600, videoInfo->vfmt->BitsPerPixel, SDL_OPENGL)) {
		Trace::error("Could not set video mode: %s\n", SDL_GetError());
		SDL_Quit();
		return false;
	}
	
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
				case SDL_QUIT:
					quit = true;
					break;
			}
		}

		int currentTime = SDL_GetTicks();
		float dt = float(currentTime - m_totalTime);

		if (!m_fixedTimeStep || dt >= m_targetElapsedTime * 1000) {
			m_totalTime = currentTime;
			update(dt);
		}

		draw();

		SDL_GL_SwapBuffers();
	}

	SDL_Quit();

	return 0;
}

void SDLApplication::update(float dt)
{
}

void SDLApplication::draw()
{
}