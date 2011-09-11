#ifndef COMMON_SDLAPPLICATION
#define COMMON_SDLAPPLICATION

#include "Application.h"
#include <SDL/SDL.h>

class SDLApplication : public Application
{
public:
	SDLApplication()
		: m_totalTime(0), m_fixedTimeStep(false), m_targetElapsedTime(1.0f/60.0f)
	{
	}

	virtual ~SDLApplication() {}

	virtual bool init(int argc, char * argv[]);
	int run();

	virtual void update(float dt);
	virtual void draw();

private:

	bool m_fixedTimeStep;
	float m_targetElapsedTime;
	int m_totalTime;
};

#endif