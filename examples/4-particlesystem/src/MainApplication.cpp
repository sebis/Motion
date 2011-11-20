#include "MainApplication.h"
#include "Material.h"
#include "MeshObject.h"
#include "Trace.h"
#include "Utils.h"

#include <GL/glew.h>

#include <cmath>

namespace ParticlePhysicsDemo
{
	using namespace Common;

	MainApplication::MainApplication(const char * title, bool fixedTimeStep, float targetElapsedTime)
		: Base(title, fixedTimeStep, targetElapsedTime),
		m_camera(glm::vec3(10.0f), glm::vec3(0.0f))
	{
	}

	MainApplication::~MainApplication()
	{
		delete m_particleSystem;
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

		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		Common::GameObject::s_camera = &m_camera;

		ParticleSettings fire;
		fire.texture = "resources/fire.bmp";

		fire.maxParticles = 5000;
		fire.gravity = glm::vec3(0.0f, 1.5f, 0.0f);

		fire.duration = 2.0f;

		fire.minVelocity = glm::vec3(-1.0f, 0.0f, -1.0f);
		fire.maxVelocity = glm::vec3(1.0f, 1.0f, 1.0f);

		fire.minStartSize = 0.5f;
		fire.maxStartSize = 1.0f;

		fire.minEndSize = 1.0f;
		fire.maxEndSize = 4.0f;

		fire.minColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.05f);
		fire.maxColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.15f);

		m_particleSystem = new ParticleSystem(fire);

		ParticleSettings smoke;
		smoke.texture = "resources/smoke.bmp";

		smoke.maxParticles = 2000;
		smoke.gravity = glm::vec3(-2.0f, -0.5f, 0.0f);
		smoke.duration = 10.0f;
		smoke.minVelocity = glm::vec3(-1.0f, 1.0f, -1.0f);
		smoke.maxVelocity = glm::vec3(1.0f, 2.0f, 1.0f);
		smoke.minStartSize = 0.4f;
		smoke.maxStartSize = 0.7f;
		smoke.minEndSize = 3.5f;
		smoke.maxEndSize = 14.0f;
		smoke.minColor = glm::vec4(1.0f);
		smoke.maxColor = glm::vec4(1.0f);

		m_smokeParticleSystem = new ParticleSystem(smoke);

		Material * grass = new Material(Shader::find("shader"));
		//grass->setTexture(new Texture("resources/grass.bmp"));
		grass->setDiffuseColor(glm::vec4(0.8f));
		grass->setSpecularColor(glm::vec4(0.2f));

		MeshObject * terrain = new MeshObject(MeshFactory::Plane(glm::vec4(1.0f), 1), grass);
		//terrain->transform().scale() = glm::vec3(50.0f);

		m_components.push_back(terrain);

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
		else if (key == Common::KEY_RESET_2)
			m_particleSystem->setShader(Shader::find("point"));
		else if (key == Common::KEY_RESET_1)
			m_particleSystem->setShader(Shader::find("particle"));
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
		default:
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
		m_camera.set_projection(glm::perspective(45.0f, float(width)/float(height), 0.1f, 1000.0f));

		m_width = width;
		m_height = height;

		Camera::s_aspect = float(width)/float(height);
	}

	void MainApplication::update(float dt)
	{
		Trace::info("update since last: %f\n", dt);
		m_camera.update(dt);

		for (ComponentIterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			(*it)->update(dt);
		}

		m_particleSystem->update(dt);
		m_smokeParticleSystem->update(dt);

		static Utils::Random r;
		float radius = 2.0f;

		for (int i = 0; i < 50; i++)
		{
			float angle = r.rand01() * 2 * M_PI;
			glm::vec3 position(radius * glm::cos(angle), 0.0f, radius * glm::sin(angle));
			m_particleSystem->addParticle(position, glm::vec3(0.0f));
		}

		for (int i = 0; i < 3; i++)
		{
			float angle = r.rand01() * 2 * M_PI;
			glm::vec3 position(radius * glm::cos(angle), 0.0f, radius * glm::sin(angle));
			m_smokeParticleSystem->addParticle(position, glm::vec3(0.0f));
		}
	}

	void MainApplication::draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (ComponentIterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			(*it)->draw();
		}

		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_smokeParticleSystem->draw();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		m_particleSystem->draw();
		
		glDisable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);

		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			Trace::error("OpenGL error: %d\n", err);
	}
}
