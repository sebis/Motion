#include "MainApplication.h"
#include "Material.h"
#include "MeshObject.h"
#include "ParticleEmitter.h"
#include "Trace.h"
#include "Utils.h"

#include <GL/glew.h>

#include <cmath>
#include <sstream>

namespace ParticlePhysicsDemo
{
	using namespace Common;

	MainApplication::MainApplication(const char * title, bool fixedTimeStep, float targetElapsedTime)
		: Base(title, fixedTimeStep, targetElapsedTime),
		m_camera(glm::vec3(10.0f), glm::vec3(0.0f)),
		m_renderPoints(false),
		m_currentScene(0),
		m_fireTrailEmitter(0),
		m_smokeTrailEmitter(0)
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

		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		Common::GameObject::s_camera = &m_camera;

		initScene(m_currentScene);

		return true;
	}

	void MainApplication::initScene(int scene)
	{
		m_components.clear();

		if (scene == 0)
		{
			// create a fire particle system

			ParticleSettings fire;
			fire.texture = "resources/fire.bmp";
			fire.maxParticles = 50000;
			fire.duration = 2.0f;
			fire.minVelocity = glm::vec3(-1.0f, 0.0f, -1.0f);
			fire.maxVelocity = glm::vec3(1.0f, 1.0f, 1.0f);
			fire.minStartSize = 0.5f;
			fire.maxStartSize = 1.0f;
			fire.minEndSize = 1.0f;
			fire.maxEndSize = 4.0f;
			fire.minColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.05f);
			fire.maxColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.15f);
			fire.srcBlend = GL_SRC_ALPHA;
			fire.dstBlend = GL_ONE;

			ParticleSystem * fireSystem = new ParticleSystem(fire);
			m_components.push_back(fireSystem);

			// create emitter for fire system
			m_components.push_back(new CircleParticleEmitter(fireSystem, 2500, 2.0f));
		
			// create separate system and emitter for user interaction
			ParticleSystem * fireTrailSystem = new ParticleSystem(fire);
			m_components.push_back(fireTrailSystem);
			delete m_fireTrailEmitter;
			m_fireTrailEmitter = new TrailParticleEmitter(fireTrailSystem, 2000);
			m_components.push_back(m_fireTrailEmitter);

			// create smoke particle system

			ParticleSettings smoke;
			smoke.texture = "resources/smoke.bmp";
			smoke.maxParticles = 2000;
			smoke.duration = 10.0f;
			smoke.minVelocity = glm::vec3(-1.0f, 1.0f, -1.0f);
			smoke.maxVelocity = glm::vec3(1.0f, 2.0f, 1.0f);
			smoke.minStartSize = 0.4f;
			smoke.maxStartSize = 0.7f;
			smoke.minEndSize = 3.5f;
			smoke.maxEndSize = 14.0f;
			smoke.minColor = glm::vec4(1.0f);
			smoke.maxColor = glm::vec4(1.0f);
			smoke.srcBlend = GL_SRC_ALPHA;
			smoke.dstBlend = GL_ONE_MINUS_SRC_ALPHA;

			ParticleSystem * smokeSystem = new ParticleSystem(smoke);
			m_components.push_back(smokeSystem);

			// add a uniform flow for smoke
			smokeSystem->addFlow(new UniformFlow(glm::vec3(-2.0f, 0.0f, 0.0f)));

			// create smoke emitter
			m_components.push_back(new CircleParticleEmitter(smokeSystem, 180, 2.0f));

			// create a separate system and emitter for interaction
			ParticleSystem * smokeTrailSystem = new ParticleSystem(smoke);
			m_components.push_back(smokeTrailSystem);
			delete m_smokeTrailEmitter;
			m_smokeTrailEmitter = new TrailParticleEmitter(smokeTrailSystem, 180);
			m_components.push_back(m_smokeTrailEmitter);

			std::stringstream ss;
			ss << "ParticleSystem Fluid Demo: Fire and smoke" << std::endl;
			ss << "Smoke is directed by uniform flow" << std::endl;
			ss << std::endl;
			ss << "Drag with right mouse to create own flames. <Space> for next demo." << std::endl;
			text = ss.str();

		}
		else if (scene == 1)
		{
			ParticleSettings water;
			water.texture = "resources/blue.bmp";
			water.maxParticles = 100000;
			water.duration = 15.0f;
			water.minVelocity = glm::vec3(0.0f, 0.0f, 1.0f);
			water.maxVelocity = glm::vec3(0.0f, 0.0f, 2.0f);
			water.minStartSize = 0.75f;
			water.maxStartSize = 0.75f;
			water.minEndSize = 0.75f;
			water.maxEndSize = 0.75f;
			water.minColor = glm::vec4(0.8f, 0.8f, 1.0f, 0.5f);
			water.maxColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.6f);
			water.srcBlend = GL_SRC_ALPHA;
			water.dstBlend = GL_ONE_MINUS_SRC_ALPHA;

			ParticleSystem * waterSystem = new ParticleSystem(water);

			// add some flow primitives
			waterSystem->addFlow(new UniformFlow(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 5.0f, 0.0f), 4.0f));
			waterSystem->addFlow(new SourceFlow(glm::vec3(0.0f, 2.0f, 0.0f), 1.0f, 15.0f));

			m_components.push_back(waterSystem);
			m_components.push_back(new ParticleEmitter(waterSystem, 2500, glm::vec3(0.0f, 5.0f, -10.0f), glm::vec3(3.0f, 0.0f, 0.0f)));

			std::stringstream ss;
			ss << "ParticleSystem Fluid Demo: Waterfall" << std::endl;
			ss << "Local uniform flow causes waterfall to fall. Source flow acts as an obstacle." << std::endl;
			ss << std::endl;
			ss << "<Space> for next demo" << std::endl;
			text = ss.str();
		}
		else if (scene == 2)
		{
			ParticleSettings leaves;
			leaves.texture = "resources/leaf.bmp";
			leaves.maxParticles = 1000;
			leaves.duration = 120.0f;
			leaves.minVelocity = glm::vec3(0.0f);
			leaves.maxVelocity = glm::vec3(0.0f);
			leaves.minStartSize = 0.5f;
			leaves.maxStartSize = 1.0f;
			leaves.minEndSize = 0.5f;
			leaves.maxEndSize = 1.0f;
			leaves.minColor = glm::vec4(0.8f, 0.8f, 0.4f, 1.0f);
			leaves.maxColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			leaves.srcBlend = GL_SRC_ALPHA;
			leaves.dstBlend = GL_ONE_MINUS_SRC_ALPHA;

			ParticleSystem * leafSystem = new ParticleSystem(leaves);

			leafSystem->addFlow(new VortexFlow(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 5.0f, 5.0f));
			leafSystem->addFlow(new SourceFlow(glm::vec3(0.0f), 10.0f, -50.0f));
		
			m_components.push_back(leafSystem);
			m_components.push_back(new ParticleEmitter(leafSystem, 75, glm::vec3(0.0f), glm::vec3(1.0f)));

			std::stringstream ss;
			ss << "ParticleSystem Fluid Demo: Leaves" << std::endl;
			ss << "Vortex flow spins leaves around a center point. Sink flow keeps leaves within the vortex." << std::endl;
			ss << std::endl;
			ss << "<Space> for first demo" << std::endl;
			text = ss.str();
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
		else if (key == Common::KEY_CONTINUE) {
			m_currentScene++;
			if (m_currentScene > 2)
				m_currentScene = 0;
			initScene(m_currentScene);
		}
		else if (key == Common::KEY_RESET_1) {
			// TODO: hack, should implement messaging
			m_renderPoints = !m_renderPoints;
			Shader * shader = Shader::find(m_renderPoints ? "point" : "particle");

			for (ComponentIterator it = m_components.begin(); it != m_components.end(); ++it)
			{
				ParticleSystem * system = dynamic_cast<ParticleSystem*>(*it);
				if (system)
					system->setShader(shader);
			}
		}
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
		else if (key == Common::KEY_MOUSE_RIGHT) {
			// transform picking point from screen coordinate to world coordinates
			glm::vec3 nearScreen(x, m_height-y, 0.0f);
			glm::vec3 farScreen(x, m_height-y, 1.0f);

			glm::vec3 nearWorld = glm::unProject(nearScreen, m_camera.view(), m_camera.projection(), glm::vec4(0, 0, m_width, m_height));
			glm::vec3 farWorld = glm::unProject(farScreen, m_camera.view(), m_camera.projection(), glm::vec4(0, 0, m_width, m_height));

			// get intersection with y = 0 plane
			glm::vec3 v = farWorld - nearWorld;
			if (v.y) {
				float t = -nearWorld.y / v.y;
				glm::vec3 position = nearWorld + t * v;

				if (m_fireTrailEmitter)
					m_fireTrailEmitter->addPosition(position);
				if (m_smokeTrailEmitter)
					m_smokeTrailEmitter->addPosition(position);
			}
		}
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
		m_camera.update(dt);

		for (ComponentIterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			(*it)->update(dt);
		}
	}

	void MainApplication::draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (ComponentIterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			(*it)->draw();
		}

		display_text(text.c_str(), 10, 15);

		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			Trace::error("OpenGL error: %d\n", err);
	}
}
