#include "Animator.h"
#include "Collider.h"
#include "CollisionDetector.h"
#include "GameObject.h"
#include "MainApplication.h"
#include "Material.h"
#include "MeshObject.h"
#include "RigidBody.h"
#include "Skeleton.h"
#include "Texture.h"
#include "Trace.h"
#include "Utils.h"

#include "KeyframeAnimator.h"
#include "Interpolator.h"

#include <GL/glew.h>
#include "glm/gtc/matrix_transform.hpp"

namespace RigidBodyDemo
{
	using namespace Common;
	using namespace Interpolation;

	MainApplication::MainApplication(bool fixedTimeStep, float targetElapsedTime)
		: Base(fixedTimeStep, targetElapsedTime),
		m_camera(glm::vec3(10.0f), glm::vec3(0.0f))
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

		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		Common::GameObject::s_camera = &m_camera;

		setupSimulation();

		return true;
	}

	void MainApplication::setupSimulation()
	{
		m_started = false;

		/// Create floor

		Material * floorMaterial = new Material(Shader::find("shader"));
		floorMaterial->setTexture(new Texture("resources/dirt.bmp"));
		floorMaterial->setDiffuseColor(glm::vec4(0.8f));
		floorMaterial->setSpecularColor(glm::vec4(0.2f));

		MeshObject * floor = new MeshObject(MeshFactory::Plane(glm::vec4(1.0f), 50), floorMaterial);
		floor->transform().scale() = glm::vec3(500.0f);

		PlaneCollider * floorCollider = new PlaneCollider(floor, 0);
		floorCollider->m_normal = glm::vec3(0.0f, 1.0f, 0.0f);
		floorCollider->m_d = 0.0f;
		CollisionDetector::instance()->addCollider(floorCollider);

		m_components.push_back(floor);

		// Create some walls

		PlaneCollider * wall1 = new PlaneCollider(0);
		wall1->m_normal = glm::vec3(-1.0f, 0.0f, 0.0f);
		wall1->m_d = -10.0f;
		CollisionDetector::instance()->addCollider(wall1);

		PlaneCollider * wall2 = new PlaneCollider(0);
		wall2->m_normal = glm::vec3(1.0f, 0.0f, 0.0f);
		wall2->m_d = -10.0f;
		CollisionDetector::instance()->addCollider(wall2);

		/// Create cube

		Material * cubeMaterial = new Material(Shader::find("shader"));
		cubeMaterial->setTexture(new Texture("resources/2.bmp"));
		cubeMaterial->setDiffuseColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		MeshObject * cube = new MeshObject(MeshFactory::Sphere(), cubeMaterial);
		cube->transform().translate(glm::vec3(5.0f, 15.0f, 0.0f));
		cube->m_rigidbody = new RigidBody(cube);

		SphereCollider * cubeCollider = new SphereCollider(cube, cube->m_rigidbody);
		cubeCollider->m_radius = 1.0f;
		CollisionDetector::instance()->addCollider(cubeCollider);

		m_components.push_back(cube);

		m_physics.addObject(cube->m_rigidbody);

		/// Create another cube

		Material * cubeMaterial2 = new Material(Shader::find("shader"));
		cubeMaterial2->setDiffuseColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		cubeMaterial2->setTexture(new Texture("resources/15.bmp"));

		MeshObject * cube2 = new MeshObject(MeshFactory::Sphere(), cubeMaterial2);
		// TODO: transform shouldn't need to be set before rigidbody
		cube2->transform().translate(glm::vec3(0.0f, 10.0f, 0.0f));
		cube2->m_rigidbody = new RigidBody(cube2);

		SphereCollider * cubeCollider2 = new SphereCollider(cube2, cube2->m_rigidbody);
		cubeCollider2->m_radius = 1.0f;
		CollisionDetector::instance()->addCollider(cubeCollider2);

		m_components.push_back(cube2);

		m_physics.addObject(cube2->m_rigidbody);
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
			m_camera.reset(glm::vec3(13.0f, 14.0f, -15.0f), glm::vec3(-2.0f, 0.0f, -2.0f));
		else if (key == Common::KEY_CONTINUE)
			m_started = true;
		else if (key == Common::KEY_RESET_1)
			m_components[1]->m_rigidbody->applyTorque(glm::vec3(0.0f, 0.0f, 1000.0f));
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
	}

	void MainApplication::update(float dt)
	{
		m_camera.update(dt);

		if (!m_started)
			return;

		for (ComponentIterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			(*it)->update(dt);
		}

		m_physics.step(dt);

		// check for GL errors
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			Trace::error("OpenGL error: %d\n", err);
	}

	void MainApplication::draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (ComponentIterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			(*it)->draw();
		}
	}
}
