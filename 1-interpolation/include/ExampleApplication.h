#ifndef EXAMPLE_APPLICATION
#define EXAMPLE_APPLICATION

#if 1
#include <SDLApplication.h>
typedef SDLApplication Base;
#else
#include <GlutApplication.h>
typedef GlutApplication Base;
#endif

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
};

#endif /* EXAMPLE_APPLICATION */