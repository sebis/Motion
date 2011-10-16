#include "MeshObject.h"
#include "Interpolator.h"
#include "KeyframeAnimator.h"
#include "MainApplication.h"
#include "Texture.h"
#include "Trace.h"
#include "Utils.h"

#include <GL/glew.h>
#include "glm/gtc/matrix_transform.hpp"

namespace Interpolation
{
	MainApplication::MainApplication(bool fixedTimeStep, float targetElapsedTime)
		: Base(fixedTimeStep, targetElapsedTime),
		m_camera(glm::vec3(13.0f, 14.0f, -15.0f), glm::vec3(-2.0f, 0.0f, -2.0f)),
		m_currentScene(0)
	{
	}

	MainApplication::~MainApplication()
	{
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

		glEnable(GL_PROGRAM_POINT_SIZE);

		// TODO: Shouldn't need to "initialize" uniforms like this
		Shader * shader = Shader::find("lambert");
		shader->bind();
		shader->setUniform("lightDirection", glm::vec3(1.0f, 0.5f, 0.25f));
		shader->unbind();

		shader = Shader::find("phong");
		shader->bind();
		shader->setUniform("lightDirection", glm::vec3(1.0f, 0.5f, 0.25f));
		shader->unbind();

		setScene(m_currentScene);

		return true;
	}

	void MainApplication::setScene(int scene)
	{
		Trace::info("Switched to scene %d\n", scene);

		// clear old scene
		for (ComponentIterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			delete *it;
		}
		m_components.clear();

		if (scene == 0)
		{
			Common::GameObject * obj = new MeshObject(Shader::find("model"), "resources/space_frigate.ply", "resources/space_frigate.bmp");
			obj->m_camera = &m_camera;
			obj->m_transform.scale() *= 0.1f;

			// create a linear interpolator for vec3 type
			Interpolator<glm::vec3> * interpolator = new LinearInterpolator<glm::vec3>;
			// create a keyframe animator that animates position
			KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(obj, interpolator, obj->m_transform.position(), false);

			animator->setRenderer(new SplineRenderer(obj, interpolator, animator->keys()));

			animator->addKeyframe(0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
			animator->addKeyframe(2000.0f, glm::vec3(3.5f, 0.0f, 4.0f));
			animator->addKeyframe(3000.0f, glm::vec3(8.0f, 0.0f, 2.5f));
			animator->addKeyframe(4000.0f, glm::vec3(7.0f, 0.0f, 0.5f));
			animator->addKeyframe(7000.0f, glm::vec3(5.0f, 0.0f, -0.5f));
			animator->addKeyframe(13000.0f, glm::vec3(-0.5f, 3.0f, 6.0f));
			animator->addKeyframe(15000.0f, glm::vec3(-9.0f, 0.0f, 3.0f));
			animator->addKeyframe(16000.0f, glm::vec3(-6.0f, 2.0f, -8.0f));
			animator->addKeyframe(18000.0f, glm::vec3(-3.0f, 0.0f, -8.0f));
			animator->addKeyframe(20000.0f, glm::vec3(-4.0f, 0.0f, -13.0f));
			animator->addKeyframe(21000.0f, glm::vec3(-1.0f, 0.0f, -12.0f));
			animator->addKeyframe(23000.0f, glm::vec3(4.0f, 0.0f, -6.0f));
			animator->addKeyframe(24000.0f, glm::vec3(0.0f, 0.0f, 0.0f));

			obj->m_animator = animator;

			m_components.push_back(obj);
		}
		else if (scene == 1)
		{
			Common::GameObject * obj = new MeshObject(Shader::find("model"), "resources/space_frigate.ply", "resources/space_frigate.bmp");
			obj->m_camera = &m_camera;
			obj->m_transform.scale() *= 0.1f;

			Interpolator<glm::vec3> * interpolator = new LinearInterpolator<glm::vec3>;
			KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(obj, interpolator, obj->m_transform.position(), true);

			animator->setRenderer(new SplineRenderer(obj, interpolator, animator->keys()));

			animator->addKeyframe(0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
			animator->addKeyframe(2000.0f, glm::vec3(3.5f, 0.0f, 4.0f));
			animator->addKeyframe(3000.0f, glm::vec3(8.0f, 0.0f, 2.5f));
			animator->addKeyframe(4000.0f, glm::vec3(7.0f, 0.0f, 0.5f));
			animator->addKeyframe(7000.0f, glm::vec3(5.0f, 0.0f, -0.5f));
			animator->addKeyframe(13000.0f, glm::vec3(-0.5f, 3.0f, 6.0f));
			animator->addKeyframe(15000.0f, glm::vec3(-9.0f, 0.0f, 3.0f));
			animator->addKeyframe(16000.0f, glm::vec3(-6.0f, 2.0f, -8.0f));
			animator->addKeyframe(18000.0f, glm::vec3(-3.0f, 0.0f, -8.0f));
			animator->addKeyframe(20000.0f, glm::vec3(-4.0f, 0.0f, -13.0f));
			animator->addKeyframe(21000.0f, glm::vec3(-1.0f, 0.0f, -12.0f));
			animator->addKeyframe(23000.0f, glm::vec3(4.0f, 0.0f, -6.0f));
			animator->addKeyframe(24000.0f, glm::vec3(0.0f, 0.0f, 0.0f));

			obj->m_animator = animator;

			m_components.push_back(obj);
		}
		else if (scene == 2)
		{
			Common::GameObject * obj = new MeshObject(Shader::find("model"), "resources/space_frigate.ply", "resources/space_frigate.bmp");
			obj->m_camera = &m_camera;
			obj->m_transform.scale() *= 0.1f;

			Interpolator<glm::vec3> * interpolator = new CatmullRomInterpolator<glm::vec3>;
			KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(obj, interpolator, obj->m_transform.position(), false, true);

			animator->setRenderer(new SplineRenderer(obj, interpolator, animator->keys()));

			animator->addKeyframe(0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
			animator->addKeyframe(2000.0f, glm::vec3(3.5f, 0.0f, 4.0f));
			animator->addKeyframe(3000.0f, glm::vec3(8.0f, 0.0f, 2.5f));
			animator->addKeyframe(4000.0f, glm::vec3(7.0f, 0.0f, 0.5f));
			animator->addKeyframe(7000.0f, glm::vec3(5.0f, 0.0f, -0.5f));
			animator->addKeyframe(13000.0f, glm::vec3(-0.5f, 3.0f, 6.0f));
			animator->addKeyframe(15000.0f, glm::vec3(-9.0f, 0.0f, 3.0f));
			animator->addKeyframe(16000.0f, glm::vec3(-6.0f, 2.0f, -8.0f));
			animator->addKeyframe(18000.0f, glm::vec3(-3.0f, 0.0f, -8.0f));
			animator->addKeyframe(20000.0f, glm::vec3(-4.0f, 0.0f, -13.0f));
			animator->addKeyframe(21000.0f, glm::vec3(-1.0f, 0.0f, -12.0f));
			animator->addKeyframe(23000.0f, glm::vec3(4.0f, 0.0f, -6.0f));
			animator->addKeyframe(24000.0f, glm::vec3(0.0f, 0.0f, 0.0f));

			obj->m_animator = animator;

			m_components.push_back(obj);
		} 
		else if (scene == 3)
		{
			Common::GameObject * obj = new MeshObject(Shader::find("model"), "resources/space_frigate.ply", "resources/space_frigate.bmp");
			obj->m_camera = &m_camera;
			obj->m_transform.scale() *= 0.1f;

			Interpolator<glm::vec3> * interpolator = new CatmullRomInterpolator<glm::vec3>;
			KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(obj, interpolator, obj->m_transform.position(), true, true);

			animator->setRenderer(new SplineRenderer(obj, interpolator, animator->keys()));

			animator->addKeyframe(0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
			animator->addKeyframe(2000.0f, glm::vec3(3.5f, 0.0f, 4.0f));
			animator->addKeyframe(3000.0f, glm::vec3(8.0f, 0.0f, 2.5f));
			animator->addKeyframe(4000.0f, glm::vec3(7.0f, 0.0f, 0.5f));
			animator->addKeyframe(7000.0f, glm::vec3(5.0f, 0.0f, -0.5f));
			animator->addKeyframe(13000.0f, glm::vec3(-0.5f, 3.0f, 6.0f));
			animator->addKeyframe(15000.0f, glm::vec3(-9.0f, 0.0f, 3.0f));
			animator->addKeyframe(16000.0f, glm::vec3(-6.0f, 2.0f, -8.0f));
			animator->addKeyframe(18000.0f, glm::vec3(-3.0f, 0.0f, -8.0f));
			animator->addKeyframe(20000.0f, glm::vec3(-4.0f, 0.0f, -13.0f));
			animator->addKeyframe(21000.0f, glm::vec3(-1.0f, 0.0f, -12.0f));
			animator->addKeyframe(23000.0f, glm::vec3(4.0f, 0.0f, -6.0f));
			animator->addKeyframe(24000.0f, glm::vec3(0.0f, 0.0f, 0.0f));

			obj->m_animator = animator;

			m_components.push_back(obj);
		}
		else if (scene == 4)
		{
			Utils::Random r(unsigned(this));

			Common::GameObject * obj = new MeshObject(Shader::find("model"), "resources/space_frigate.ply", "resources/space_frigate.bmp");
			obj->m_camera = &m_camera;
			obj->m_transform.scale() *= 0.1f;

			Interpolator<glm::vec3> * interpolator = new KochanekBartelsInterpolator<glm::vec3>;
			KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(obj, interpolator, obj->m_transform.position(), true, true);

			animator->setRenderer(new SplineRenderer(obj, interpolator, animator->keys()));

			animator->addKeyframe(0.0f, glm::vec3(0.0f, 0.0f, 0.0f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(2000.0f, glm::vec3(3.5f, 0.0f, 4.0f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(3000.0f, glm::vec3(8.0f, 0.0f, 2.5f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(4000.0f, glm::vec3(7.0f, 0.0f, 0.5f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(7000.0f, glm::vec3(5.0f, 0.0f, -0.5f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(13000.0f, glm::vec3(-0.5f, 3.0f, 6.0f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(15000.0f, glm::vec3(-9.0f, 0.0f, 3.0f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(16000.0f, glm::vec3(-6.0f, 2.0f, -8.0f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(18000.0f, glm::vec3(-3.0f, 0.0f, -8.0f), 0, -1, 0);
			animator->addKeyframe(20000.0f, glm::vec3(-4.0f, 0.0f, -13.0f), 0, -1, 0);
			animator->addKeyframe(21000.0f, glm::vec3(-1.0f, 0.0f, -12.0f), 0, -1, 0);
			animator->addKeyframe(23000.0f, glm::vec3(4.0f, 0.0f, -6.0f), 0, -1, 0);
			animator->addKeyframe(24000.0f, glm::vec3(0.0f, 0.0f, 0.0f), 0, -1, 0);

			obj->m_animator = animator;

			m_components.push_back(obj);
		}
		else if (scene == 4)
		{
			Common::GameObject * cube = new MeshObject(Shader::find("lambert"), Common::MeshFactory::Cube());
			cube->m_camera = &m_camera;
			cube->m_transform.translate(glm::vec3(-10.0f, 0.0f, 0.0f));

			Interpolator<glm::vec3> * interpolator = new LinearInterpolator<glm::vec3>;
			KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(cube, interpolator, cube->m_transform.rotation(), false, false, false);

			animator->addKeyframe(0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
			animator->addKeyframe(2000.0f, glm::vec3(90.0f, 0.0f, 0.0f));
			animator->addKeyframe(4000.0f, glm::vec3(90.0f, 90.0f, 0.0f));
			animator->addKeyframe(6000.0f, glm::vec3(90.0f, 90.0f, 90.0f));
			animator->addKeyframe(8000.0f, glm::vec3(0.0f, 0.0f, 0.0f));

			cube->m_animator = animator;

			m_components.push_back(cube);
		}
		else if (scene == 5)
		{
			Common::GameObject * cube = new MeshObject(Shader::find("lambert"), Common::MeshFactory::Cube());
			cube->m_camera = &m_camera;
			cube->m_transform.useQuaternions() = true;
			cube->m_transform.translate(glm::vec3(6.0f, 0.0f, -4.0f));

			Interpolator<glm::quat> * interpolator = new SphericalLinearInterpolator<glm::quat>;
			KeyframeAnimator<glm::quat> * animator = new KeyframeAnimator<glm::quat>(cube, interpolator, cube->m_transform.quaternion(), false, false, false);

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
		else if (scene == 6)
		{
			Common::GameObject * cube = new MeshObject(Shader::find("phong"), Common::MeshFactory::Cube());
			cube->m_camera = &m_camera;
			cube->m_transform.useQuaternions() = true;
			cube->m_transform.translate(glm::vec3(0.0f, 0.0f, -10.0f));

			Interpolator<glm::quat> * interpolator = new BezierSphericalLinearInterpolator<glm::quat>;
			KeyframeAnimator<glm::quat> * animator = new KeyframeAnimator<glm::quat>(cube, interpolator, cube->m_transform.quaternion(), false, false, false);

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
		else
		{
			Trace::error("No scene %d found!\n", scene);
		}
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
		else if (key == Common::KEY_CONTINUE)
			setScene(++m_currentScene);
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

		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			Trace::error("OpenGL error: %d\n", err);
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