#include "ExampleApplication.h"
#include "Trace.h"

#include <gl/glew.h>
#include "glm/gtc/matrix_transform.hpp"

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
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	m_cube = MeshFactory::Cube(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	m_shader = new Shader("resources/solid.vert", "resources/solid.frag");

	m_shader->bindAttribLocation(0, "in_Position");
	m_shader->bindAttribLocation(1, "in_Normal");
	m_shader->bindAttribLocation(2, "in_Color");

	m_shader->link();

	m_shader->bind();
	m_shader->setUniform("world", glm::mat4(1.0f));
	m_shader->setUniform("view", glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	m_shader->setUniform("projection", glm::perspective(45.0f, 800.0f/600.0f, 0.1f, 100.0f));
	m_shader->setUniform("lightDirection", glm::vec3(1.0f, 0.5f, 0.25f));
	m_shader->setUniform("eyePosition", glm::vec3(10.0f, 10.0f, 10.0f));
	m_shader->unbind();

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
	
	m_shader->bind();

	m_cube.draw();

	m_shader->unbind();

	/*glViewport(0, 0, 800, 600);

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

	glEnd();*/

}