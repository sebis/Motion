#include "MainApplication.h"
#include "Material.h"
#include "MeshObject.h"
#include "ParticleEmitter.h"
#include "Trace.h"
#include "Utils.h"

#include <GL/glew.h>

#include <cmath>

namespace ParticlePhysicsDemo
{
	using namespace Common;

	MainApplication::MainApplication(const char * title, bool fixedTimeStep, float targetElapsedTime)
		: Base(title, fixedTimeStep, targetElapsedTime),
		m_camera(glm::vec3(10.0f), glm::vec3(0.0f)),
		m_renderPoints(false)
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

		/*Material * metal = new Material(Shader::find("shader"));
		metal->setDiffuseColor(glm::vec4(0.6f, 0.5f, 0.5f, 1.0f));
		metal->setSpecularColor(glm::vec4(0.6f, 0.5f, 0.5f, 1.0f));

		MeshObject * buddha = new MeshObject(MeshFactory::FromFile("resources/happy.ply"), metal);
		buddha->transform().scale() = glm::vec3(10.0f);

		m_components.push_back(buddha);*/

		/*Material * metal = new Material(Shader::find("shader"));
		metal->setDiffuseColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		MeshObject * ball = new MeshObject(MeshFactory::Sphere(), metal);
		ball->transform().scale() = glm::vec3(1.0f);
		ball->transform().position() = glm::vec3(10.0f, 2.0f, -8.0f);
		m_components.push_back(ball);*/

		fire.texture = "resources/fire.bmp";
		fire.maxParticles = 50000;
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
		fire.srcBlend = GL_SRC_ALPHA;
		fire.dstBlend = GL_ONE;

		ParticleSystem * fireSystem = new ParticleSystem(fire);
		m_components.push_back(fireSystem);
		m_components.push_back(new CircleParticleEmitter(fireSystem, 2500, 2.0f));
		
		ParticleSystem * fireTrailSystem = new ParticleSystem(fire);
		m_components.push_back(fireTrailSystem);
		m_fireTrailEmitter = new TrailParticleEmitter(fireTrailSystem, 2000);
		m_components.push_back(m_fireTrailEmitter);

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
		smoke.srcBlend = GL_SRC_ALPHA;
		smoke.dstBlend = GL_ONE_MINUS_SRC_ALPHA;

		ParticleSystem * smokeSystem = new ParticleSystem(smoke);
		smokeSystem->addFlow(new UniformFlow(glm::vec3(-2.0f, 0.0f, 0.0f)));
		m_components.push_back(smokeSystem);
		m_components.push_back(new CircleParticleEmitter(smokeSystem, 180, 2.0f));

		ParticleSystem * smokeTrailSystem = new ParticleSystem(smoke);
		m_components.push_back(smokeTrailSystem);
		m_smokeTrailEmitter = new TrailParticleEmitter(smokeTrailSystem, 180);
		m_components.push_back(m_smokeTrailEmitter);

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
		//waterSystem->addFlow(new VortexFlow(glm::vec3(8.0f, 0.0f, -4.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, 5.0f));
		waterSystem->addFlow(new UniformFlow(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(8.0f, 5.0f, -10.0f), 4.0f));
		waterSystem->addFlow(new SourceFlow(glm::vec3(8.0f, 2.0f, -10.0f), 1.0f, 15.0f));
		//waterSystem->addFlow(new UniformFlow(glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(10.0f, 0.0f, -8.0f), 4.0f));

		m_components.push_back(waterSystem);
		m_components.push_back(new ParticleEmitter(waterSystem, 2500, glm::vec3(8.0f, 5.0f, -20.0f), glm::vec3(3.0f, 0.0f, 0.0f)));

		leaves.texture = "resources/leaf.bmp";
		leaves.maxParticles = 1000;
		leaves.duration = 120.0f;
		leaves.minVelocity = glm::vec3(0.0f);
		leaves.maxVelocity = glm::vec3(0.0f);
		leaves.minStartSize = 0.5f;
		leaves.maxStartSize = 1.0f;
		leaves.minEndSize = 0.5f;
		leaves.maxEndSize = 1.0f;
		leaves.minColor = glm::vec4(1.0f, 1.0f, 0.5f, 1.0f);
		leaves.maxColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		leaves.srcBlend = GL_SRC_ALPHA;
		leaves.dstBlend = GL_ONE_MINUS_SRC_ALPHA;

		ParticleSystem * leafSystem = new ParticleSystem(leaves);
		leafSystem->addFlow(new VortexFlow(glm::vec3(-10.0f, 0.0f, 15.0f), glm::vec3(0.0f, 1.0f, 0.0f), 5.0f, 5.0f));
		leafSystem->addFlow(new SourceFlow(glm::vec3(-10.0f, 0.0f, 15.0f), 10.0f, -50.0f));
		
		m_components.push_back(leafSystem);
		m_components.push_back(new ParticleEmitter(leafSystem, 75, glm::vec3(-10.0f, 0.0f, 15.0f), glm::vec3(1.0f)));

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
		else if (key == Common::KEY_RESET_1) {
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
			glm::vec3 nearScreen(x, m_height-y, 0.0f);
			glm::vec3 farScreen(x, m_height-y, 1.0f);

			glm::vec3 nearWorld = glm::unProject(nearScreen, m_camera.view(), m_camera.projection(), glm::vec4(0, 0, m_width, m_height));
			glm::vec3 farWorld = glm::unProject(farScreen, m_camera.view(), m_camera.projection(), glm::vec4(0, 0, m_width, m_height));

			glm::vec3 v = farWorld - nearWorld;
			float t = -nearWorld.y / v.y;
			glm::vec3 position = nearWorld + t * v;

			m_fireTrailEmitter->addPosition(position);
			m_smokeTrailEmitter->addPosition(position);
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

		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			Trace::error("OpenGL error: %d\n", err);
	}
}
