#ifndef EXAMPLE_APPLICATION
#define EXAMPLE_APPLICATION

#if 0
#include <SDLApplication.h>
typedef SDLApplication Base;
#else
#include <GlutApplication.h>
typedef GlutApplication Base;
#endif

#include "Mesh.h"
#include "Shader.h"

class ExampleApplication : public Base
{
private:

public:
	ExampleApplication();

	bool init(int argc, char * argv[]);
	void update(float dt);
	void draw();

private:
	float m_rotation;

	Mesh m_cube;
	Shader * m_shader;
};

#endif /* EXAMPLE_APPLICATION */