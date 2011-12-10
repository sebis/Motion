#include "MainApplication.h"
#include "CollisionDetector.h"
#include "Material.h"
#include "MeshObject.h"
#include "SoftBody.h"
#include "SoftBodyWorld.h"
#include "Trace.h"
#include "Utils.h"

#include <GL/glew.h>

namespace SoftBodyDemo
{
	using namespace Common;

	namespace
	{
		SoftBodyWorld g_world;
	}

	MainApplication::MainApplication(const char * title, bool fixedTimeStep, float targetElapsedTime)
		: Base(title, fixedTimeStep, targetElapsedTime),
		m_camera(glm::vec3(10.0f), glm::vec3(0.0f)),
		m_started(false)
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

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClearDepth(1.0f);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		Common::GameObject::s_camera = &m_camera;

		Material * yellowMaterial = new Material(Shader::find("shader"));
		//yellowMaterial->setAmbientColor(glm::vec4(1, 0, 0, 1));
		//yellowMaterial->setWireframe(true);
		//MeshObject * cube = new MeshObject(MeshFactory::FromFile("resources/goomba.ply"), yellowMaterial);
		MeshObject * cube = new MeshObject(MeshFactory::Sphere(glm::vec4(1.0f), 8), yellowMaterial);
		cube->transform().translate(glm::vec3(0.0f, 0.0f, 0.0f));
		//cube->transform().scale() = glm::vec3(0.4f);

		Mesh * lowpoly = MeshFactory::Sphere(glm::vec4(1.0f), 8);
		//Mesh * lowpoly = MeshFactory::FromFile("resources/goomba-low.ply");

		MeshCollider * cubeCollider = new MeshCollider(cube);
		cubeCollider->m_mesh = lowpoly;
		cubeCollider->m_bvh = BVH::constructFromMesh(lowpoly);
		CollisionDetector::instance()->addCollider(cubeCollider);

		m_components.push_back(cube);

		Material * clothMaterial = new Material(Shader::find("shader"));
		clothMaterial->setTexture(new Texture("resources/scarf.bmp"));
		clothMaterial->setDiffuseColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		//MeshObject * cloth = new MeshObject(MeshFactory::PlaneMesh(), clothMaterial);
		SoftBody * body = 0;
		MeshObject * cloth = SoftBody::createCloth(clothMaterial, &g_world, body);
		cloth->transform().translate(glm::vec3(-2.5f, 5.0f, -2.5f));
		//cloth->transform().scale() = glm::vec3(5.0f);

		/*MeshCollider * clothCollider = new MeshCollider(cloth, body, true);
		clothCollider->m_mesh = cloth->mesh();
		CollisionDetector::instance()->addCollider(clothCollider);*/

		m_components.push_back(cloth);

		//addDrawDebug(cubeCollider->m_bvh);

		return true;
	}

	void MainApplication::addDrawDebug(BVH * bvh)
	{
		BVHNode * root = bvh->root();

		Material * wireframe = new Material(Shader::find("shader"));
		wireframe->setAmbientColor(glm::vec4(1.0f));
		wireframe->setWireframe(true);

		for (std::vector<BVHNode*>::iterator it = root->m_children.begin(); it != root->m_children.end(); it++)
		{
			BVHNode * current = *it;
			//BVHNode * current = root;
			BoundingSphere * bs = dynamic_cast<BoundingSphere *>(current->m_bv);
			if (bs)
			{
				bs->print_debug();

				MeshObject * sphere = new MeshObject(MeshFactory::Sphere(glm::vec4(1.0f), 10), wireframe);

				sphere->transform().translate(bs->center());
				sphere->transform().scale() = glm::vec3(1.0f) * bs->radius();

				m_components.push_back(sphere);
			}
			//break;
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
			m_started = true;
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
		m_camera.update(dt);

		if (!m_started)
			return;

		for (ComponentIterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			(*it)->update(dt);
		}

		g_world.update(dt);
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
