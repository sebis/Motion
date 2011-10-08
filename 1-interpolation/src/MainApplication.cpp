#include "CubeObject.h"
#include "MainApplication.h"
#include "Trace.h"

#include <GL/glew.h>
#include "glm/gtc/matrix_transform.hpp"

MainApplication::MainApplication(bool fixedTimeStep, float targetElapsedTime)
	: Base(fixedTimeStep, targetElapsedTime), m_rotation(0), m_cube(0), m_shader(0)
{
}

MainApplication::~MainApplication()
{
	delete m_cube;
	delete m_shader;
}

bool MainApplication::init(int argc, char * argv[])
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
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_PROGRAM_POINT_SIZE);


	// TODO: Shouldn't need to "initialize" uniforms like this
	m_shader = Shader::find("lambert");

	m_shader->bind();
	/*m_shader->setUniform("view", glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	m_shader->setUniform("projection", glm::perspective(45.0f, 800.0f/600.0f, 0.1f, 100.0f));*/
	m_shader->setUniform("lightDirection", glm::vec3(1.0f, 0.5f, 0.25f));
	m_shader->unbind();

	/*Shader * solidShader = Shader::find("solid");

	solidShader->bind();
	solidShader->setUniform("view", glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	solidShader->setUniform("projection", glm::perspective(45.0f, 800.0f/600.0f, 0.1f, 100.0f));
	solidShader->unbind();

	Shader * pointShader = Shader::find("point");

	pointShader->bind();
	pointShader->setUniform("view", glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	pointShader->setUniform("projection", glm::perspective(45.0f, 800.0f/600.0f, 0.1f, 100.0f));
	pointShader->unbind();*/

	m_cube = new Interpolation::CubeObject(m_shader);
	m_cube->m_camera = &m_camera;

	return true;
}

void MainApplication::keyDown(Common::Key key)
{
	if (key == Common::KEY_MOVE_FORWARD)
		m_camera.raiseFlag(Common::Camera::FORWARD);
	else if (key == Common::KEY_MOVE_BACKWARD)
		m_camera.raiseFlag(Common::Camera::BACKWARD);
	else if (key == Common::KEY_MOVE_LEFT)
		m_camera.raiseFlag(Common::Camera::LEFT);
	else if (key == Common::KEY_MOVE_RIGHT)
		m_camera.raiseFlag(Common::Camera::RIGHT);
}

void MainApplication::keyUp(Common::Key key)
{
	if (key == Common::KEY_MOVE_FORWARD)
		m_camera.dropFlag(Common::Camera::FORWARD);
	else if (key == Common::KEY_MOVE_BACKWARD)
		m_camera.dropFlag(Common::Camera::BACKWARD);
	else if (key == Common::KEY_MOVE_LEFT)
		m_camera.dropFlag(Common::Camera::LEFT);
	else if (key == Common::KEY_MOVE_RIGHT)
		m_camera.dropFlag(Common::Camera::RIGHT);
}

void MainApplication::mouse(Common::Key key, int x, int y)
{
	if (key == Common::KEY_MOUSE_LEFT)
		m_camera.turn(x/float(m_width), y/float(m_height));
}

void MainApplication::window_resized(int width, int height)
{
	glViewport(0, 0, width, height);
	m_camera.set_projection(glm::perspective(45.0f, float(width)/float(height), 0.1f, 100.0f));

	m_width = width;
	m_height = height;
}

void MainApplication::update(float dt)
{
	/*Trace::info("Update (%f)\n", dt);
	static float rotation = 0.0f;
	rotation += 0.01f * dt;

	m_cube->m_transform.reset();
	m_cube->m_transform.Translate(glm::vec3(glm::sin(rotation), 0.0f, 0.0f));*/

	m_camera.update(dt);

	if (m_cube->m_animator)
		m_cube->m_animator->update(dt);

	//m_cube->m_transform.rotation() += glm::vec3(0, dt/100.0f, 0);
	//Trace::info("rotation: %f\n", m_cube->m_transform.rotation().y);
		
	/*GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		Trace::error("OpenGL error: %s\n", gluErrorString(err));*/
}

void MainApplication::draw()
{
	//Trace::info("Draw\n");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_cube->m_renderer)
		m_cube->m_renderer->draw();

	if (m_cube->m_animator)
		m_cube->m_animator->visualize();
}
