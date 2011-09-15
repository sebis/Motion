#ifndef MAIN_APPLICATION
#define MAIN_APPLICATION

#if 0
#include <SDLApplication.h>
typedef SDLApplication Base;
#else
#include <GlutApplication.h>
typedef GlutApplication Base;
#endif

#include "GameObject.h"
#include "Shader.h"

class MainApplication : public Base
{
private:

public:
	MainApplication(bool fixedTimeStep = true, float targetElapsedTime = 1.0f/60.0f);
	virtual ~MainApplication();

	bool init(int argc, char * argv[]);
	void update(float dt);
	void draw();

private:
	float m_rotation;

	Common::GameObject * m_cube;
	Shader * m_shader;
};

#endif /* MAIN_APPLICATION */