#include "CubeObject.h"
#include "Interpolator.h"
#include "KeyframeAnimator.h"
#include "MainApplication.h"
#include "Trace.h"

#include <GL/glew.h>
#include "glm/gtc/matrix_transform.hpp"

namespace Interpolation
{
	MainApplication::MainApplication(bool fixedTimeStep, float targetElapsedTime)
		: Base(fixedTimeStep, targetElapsedTime),
		m_shader(0),
		m_camera(glm::vec3(10.0f))
	{
	}

	MainApplication::~MainApplication()
	{
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
		m_shader->setUniform("lightDirection", glm::vec3(1.0f, 0.5f, 0.25f));
		m_shader->unbind();

		{
			Common::GameObject * cube = new Interpolation::CubeObject(m_shader);
			cube->m_camera = &m_camera;
			Interpolator<glm::vec3> * interpolator = new CatmullRomInterpolator<glm::vec3>;
			KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(cube, interpolator, cube->m_transform.position(), true, true);

			animator->addKeyframe(0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 0, -1, 0);
			animator->addKeyframe(2000.0f, glm::vec3(-2.5f, 0.0f, 2.0f), 0, -1, 0);
			animator->addKeyframe(6000.0f, glm::vec3(5.0f, 0.0f, 5.0f), 0, -1, 0);
			animator->addKeyframe(7000.0f, glm::vec3(3.0f, 0.0f, 3.0f), 0, -1, 0);
			animator->addKeyframe(13000.0f, glm::vec3(-1.5f, 3.0f, 8.0f), 0, -1, 0);
			animator->addKeyframe(15000.0f, glm::vec3(-4.0f, 0.0f, 7.0f), 0, -1, 0);
			animator->addKeyframe(16000.0f, glm::vec3(-5.0f, 2.0f, -7.0f), 0, -1, 0);
			//animator->addKeyframe(20000.0f, glm::vec3(5.0f, 0.0f, -7.0f), 0, -1, 0);
			animator->addKeyframe(20000.0f, glm::vec3(0.0f, 0.0f, -5.0f), 0, -1, 0);
			animator->addKeyframe(22000.0f, glm::vec3(0.0f, 0.0f, 0.0f), 0, -1, 0);

			animator->reparameterize();

			// TODO: should not need to set spline renderer AFTER keyframes
			animator->setRenderer(new SplineRenderer(cube, interpolator, *animator));

			cube->m_animator = animator;

			m_components.push_back(cube);
		}

		{
			Common::GameObject * cube = new Interpolation::CubeObject(m_shader);
			cube->m_camera = &m_camera;
			cube->m_transform.translate(glm::vec3(-10.0f, 0.0f, 0.0f));

			Interpolator<glm::vec3> * interpolator = new LinearInterpolator<glm::vec3>;
			KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(cube, interpolator, cube->m_transform.rotation(), true, false);

			animator->addKeyframe(0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
			animator->addKeyframe(2000.0f, glm::vec3(90.0f, 0.0f, 0.0f));
			animator->addKeyframe(4000.0f, glm::vec3(90.0f, 90.0f, 0.0f));
			animator->addKeyframe(6000.0f, glm::vec3(90.0f, 90.0f, 90.0f));
			animator->addKeyframe(8000.0f, glm::vec3(0.0f, 0.0f, 0.0f));

			cube->m_animator = animator;

			m_components.push_back(cube);
		}

		{
			Common::GameObject * cube = new Interpolation::CubeObject(m_shader);
			cube->m_camera = &m_camera;
			cube->m_transform.useQuaternions() = true;
			cube->m_transform.translate(glm::vec3(6.0f, 0.0f, -4.0f));

			Interpolator<glm::quat> * interpolator = new SphericalLinearInterpolator<glm::quat>;
			KeyframeAnimator<glm::quat> * animator = new KeyframeAnimator<glm::quat>(cube, interpolator, cube->m_transform.quaternion(), true, false);

			glm::quat q;
			q = glm::rotate(q, 0.0f, glm::vec3(1, 0, 0));
			animator->addKeyframe(0.0f, q);
			q = glm::rotate(q, -90.0f, glm::vec3(1, 0, 0));
			animator->addKeyframe(2000.0f, q);
			q = glm::rotate(q, 90.0f, glm::vec3(0, 1, 0));
			animator->addKeyframe(4000.0f, q);
			q = glm::rotate(q, -90.0f, glm::vec3(1, 0, 0));
			animator->addKeyframe(6000.0f, q);
			q = glm::rotate(q, 180.0f, glm::vec3(0, 1, 0));
			animator->addKeyframe(8000.0f, q);

			/*float omega = (15.0f/180.0f)*M_PI;
			float s = std::cos(omega/2);
			glm::vec3 v = std::sin(omega/2)*glm::vec3(0, 1, 0);

			animator->addKeyframe(0.0f, glm::quat(1, glm::vec3(0)));
			animator->addKeyframe(2000.0f, glm::quat(s, v));*/

			cube->m_animator = animator;

			m_components.push_back(cube);
		}

		{
			Common::GameObject * cube = new Interpolation::CubeObject(m_shader);
			cube->m_camera = &m_camera;
			cube->m_transform.useQuaternions() = true;
			cube->m_transform.translate(glm::vec3(0.0f, 0.0f, -10.0f));

			Interpolator<glm::quat> * interpolator = new BezierSphericalLinearInterpolator<glm::quat>;
			KeyframeAnimator<glm::quat> * animator = new KeyframeAnimator<glm::quat>(cube, interpolator, cube->m_transform.quaternion(), true, false);

			glm::quat q;
			q = glm::rotate(q, 0.0f, glm::vec3(1, 0, 0));
			animator->addKeyframe(0.0f, q);
			q = glm::rotate(q, -90.0f, glm::vec3(1, 0, 0));
			animator->addKeyframe(2000.0f, q);
			q = glm::rotate(q, 90.0f, glm::vec3(0, 1, 0));
			animator->addKeyframe(4000.0f, q);
			q = glm::rotate(q, -90.0f, glm::vec3(1, 0, 0));
			animator->addKeyframe(6000.0f, q);
			q = glm::rotate(q, 180.0f, glm::vec3(0, 1, 0));
			animator->addKeyframe(8000.0f, q);
			/*q = glm::rotate(q, 90.0f, glm::vec3(1, 0, 0));
			animator->addKeyframe(10000.0f, q);*/

			/*float omega = (15.0f/180.0f)*M_PI;
			float s = std::cos(omega/2);
			glm::vec3 v = std::sin(omega/2)*glm::vec3(0, 1, 0);

			animator->addKeyframe(0.0f, glm::quat(1, glm::vec3(0)));
			animator->addKeyframe(2000.0f, glm::quat(s, v));*/

			cube->m_animator = animator;

			m_components.push_back(cube);
		}

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
		switch (key)
		{
		case Common::KEY_MOVE_FORWARD:
			m_camera.dropFlag(Common::Camera::FORWARD);
			break;
		case Common::KEY_MOVE_BACKWARD:
			m_camera.dropFlag(Common::Camera::BACKWARD);
			break;
		case Common::KEY_MOVE_LEFT:
			m_camera.dropFlag(Common::Camera::LEFT);
			break;
		case Common::KEY_MOVE_RIGHT:
			m_camera.dropFlag(Common::Camera::RIGHT);
			break;
		case Common::KEY_RESET_1:
			m_camera.reset(glm::vec3(10.0f));
			break;
		case Common::KEY_RESET_2:
			m_camera.reset(glm::vec3(-10.0f));
			break;
		}
	}

	void MainApplication::mouse(Common::Key key, int x, int y)
	{
		if (key == Common::KEY_MOUSE_LEFT)
			m_camera.turn(glm::vec2(x, y));
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

		for (ComponentIterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			if ((*it)->m_animator)
				(*it)->m_animator->update(dt);
		}

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

		for (ComponentIterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			if ((*it)->m_renderer)
				(*it)->m_renderer->draw();
			if ((*it)->m_animator)
				(*it)->m_animator->visualize();
		}
	}
}