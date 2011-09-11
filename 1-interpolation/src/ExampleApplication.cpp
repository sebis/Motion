#include "ExampleApplication.h"
#include "Trace.h"

#include <gl/glew.h>
#include <glm/glm.hpp>

ExampleApplication::ExampleApplication()
	: m_rotation(0)
{
}

bool ExampleApplication::init(int argc, char * argv[])
{
	if (!Base::init(argc, argv))
		return false;

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		Trace::error("Could not initialize GLEW: %s\n", glewGetErrorString(err));
		return false;
	}

	Trace::info("OpenGL context: %s\n", glGetString(GL_VERSION));
	Trace::info("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	Trace::info("GLEW Version: %s\n", glewGetString(GLEW_VERSION));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	Trace::info("Init..\n");

	return true;
}

void ExampleApplication::update(float dt)
{
	//Trace::info("Update (%f)\n", dt);

	//m_rotation += 0.01;
}

void ExampleApplication::draw()
{
	//Trace::info("Draw\n");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glViewport(0, 0, 800, 600);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 800.0, 600.0, 0.0, 0.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_LINE_STRIP);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(100.0f, 100.0f);
	glVertex2f(200.0f, 100.0f);
	glVertex2f(200.0f, 200.0f);
	glVertex2f(100.0f, 200.0f);

	glEnd();

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		Trace::error("OpenGL error: %s\n", glewGetErrorString(err));
}