/**
 * Copyright 2011-2012 Sebastian Eriksson.
 * This file is part of Motion.
 *
 * Motion is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Motion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Motion. If not, see <http://www.gnu.org/licenses/>.
 */

#include "MainApplication.h"
#include "CollisionDetector.h"
#include "KeyframeAnimator.h"
#include "Material.h"
#include "MeshObject.h"
#include "SoftBody.h"
#include "SoftBodyWorld.h"
#include "Physics.h"
#include "Trace.h"
#include "Utils.h"

#include <GL/glew.h>

#include <iomanip>
#include <sstream>

namespace SoftBodyDemo
{
	using namespace Common;
	using namespace Interpolation;

	namespace
	{
		SoftBodyWorld g_world;
		SoftBody * g_body = 0;
	}

	MainApplication::MainApplication(const char * title, bool fixedTimeStep, float targetElapsedTime)
		: Base(title, fixedTimeStep, targetElapsedTime),
		m_camera(glm::vec3(10.0f), glm::vec3(0.0f)),
		m_started(false),
		m_currentLevel(0),
		m_debug(false),
		m_constrained(true),
		m_animate(true),
		m_help(true)
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

		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		Common::GameObject::s_camera = &m_camera;

		// Set some properties
		CollisionDetector::COLLISION_THRESHOLD = 0.075f;
		SoftBody::ITERATION_COUNT = 5;
		SoftBody::FRICTION = 0.5f;

		initScene();

		return true;
	}

	void MainApplication::initScene()
	{
		m_components.clear();
		m_debugComponents.clear();
		g_world = SoftBodyWorld();
		g_body = 0;

		// Create wood material
		Material * woodMaterial = new Material(Shader::find("shader"));
		woodMaterial->setTexture(new Texture("resources/wood.bmp"));

		// Load bowl model
		MeshObject * bowl = new MeshObject(MeshFactory::FromFile("resources/bowl.ply"), woodMaterial);
		bowl->transform().translate(glm::vec3(0.0f, 2.0f, 0.0f));
		bowl->transform().update();

		// Load low-poly model of bowl for collision detection
		Mesh * lowpoly = MeshFactory::FromFile("resources/bowl-low.ply");

		MeshCollider * bowlCollider = new MeshCollider(bowl);
		bowlCollider->m_mesh = lowpoly;
		bowlCollider->m_bvh = BVH::constructFromMesh(lowpoly);
		CollisionDetector::instance()->addCollider(bowlCollider);

		m_components.push_back(bowl);

		// Create cloth material
		Material * clothMaterial = new Material(Shader::find("shader"));
		clothMaterial->setTexture(new Texture("resources/cloth.bmp"));
		clothMaterial->setDiffuseColor(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
		clothMaterial->setAmbientColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
		clothMaterial->setSpecularColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

		// Create cloth mesh with attached soft body information
		MeshObject * cloth = SoftBody::createCloth(clothMaterial, &g_world, &g_body);
		cloth->transform().translate(glm::vec3(-2.5f, 5.0f, -2.5f));
		cloth->transform().update();

		m_components.push_back(cloth);

		// Create material for apple
		Material * appleMaterial = new Material(Shader::find("shader"));
		appleMaterial->setDiffuseColor(glm::vec4(0.9f));
		appleMaterial->setTexture(new Texture("resources/apple.bmp"));

		// Load apple model from file
		MeshObject * apple = new MeshObject(MeshFactory::FromFile("resources/apple.ply"), appleMaterial);
		apple->transform().translate(glm::vec3(0.0f, 3.0f, 10.0f));
		apple->transform().scale(glm::vec3(0.5f));

		// Create a mathematical sphere collider for the apple
		SphereCollider * sphereCollider = new SphereCollider(apple);
		CollisionDetector::instance()->addCollider(sphereCollider);

		m_components.push_back(apple);

		// Create animator that animates the apple
		KeyframeAnimator<glm::vec3> * anim = new KeyframeAnimator<glm::vec3>(apple, new LinearInterpolator<glm::vec3>, apple->transform().position());
		anim->addKeyframe(0.0f, glm::vec3(0.0f, 3.0f, 10.0f));
		anim->addKeyframe(20000.0f, glm::vec3(0.0f, 3.0f, -10.0f));
		anim->addKeyframe(40000.0f, glm::vec3(0.0f, 3.0f, 10.0f));
		m_ballAnimator = anim;

		// Create some debug spheres for the BVH
		int level = 0;
		while (true) {
			ComponentCollection components = addDrawDebug(bowlCollider, level);
			if (components.empty())
				break;

			m_debugComponents[level] = components;
			level++;
		}
	}

	namespace
	{
		void collectBvhs(std::vector<BVHNode *> & nodes, BVHNode * current, int levelFrom, int levelTo, int level = 0)
		{
			if (level >= levelFrom && level <= levelTo)
				nodes.push_back(current);

			if (level == levelTo)
				return;

			if (current->left)
				collectBvhs(nodes, current->left, levelFrom, levelTo, level+1);
			if (current->right)
				collectBvhs(nodes, current->right, levelFrom, levelTo, level+1);
		}
	}
	
	MainApplication::ComponentCollection MainApplication::addDrawDebug(MeshCollider * collider, int level)
	{
		BVHNode * root = collider->m_bvh->root();

		Material * wireframe = new Material(Shader::find("shader"));
		wireframe->setAmbientColor(glm::vec4(1.0f));
		wireframe->setWireframe(true);

		std::vector<BVHNode *> nodes;
		collectBvhs(nodes, root, level, level);

		int leaves = 0;
		ComponentCollection components;

		for (unsigned i = 0; i < nodes.size(); i++) {
			BoundingSphere * bs = nodes[i]->m_bv;
			if (nodes[i]->m_isLeaf)
				leaves++;

			MeshObject * sphere = new MeshObject(MeshFactory::Sphere(glm::vec4(1.0f), 10), wireframe);

			sphere->transform().translate(bs->c);
			sphere->transform().translate(collider->transform().position());
			sphere->transform().scale(glm::vec3(1.0f) * bs->r);

			components.push_back(sphere);
		}

		Trace::info("Nodes at level %d: %d (%d leaves)\n", level, nodes.size(), leaves);

		return components;
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
			if (m_started)
				initScene();
			m_started = !m_started;
		}
		else if (key == Common::KEY_RESET_2)
			m_constrained = !m_constrained;
		else if (key == Common::KEY_VERBOSE)
			m_animate = !m_animate;
		else if (key == Common::KEY_NEXT)
			m_currentLevel++;
		else if (key == Common::KEY_PREV)
			m_currentLevel = m_currentLevel > 0 ? m_currentLevel-1 : 0;
		else if (key == Common::KEY_RESET_1)
			m_debug = !m_debug;
		else if (key == Common::KEY_HELP)
			m_help = !m_help;

		else if (key == Common::KEY_1)
			SoftBody::WIND.x += 0.1f;
		else if (key == Common::KEY_2)
			SoftBody::WIND.x = 0.0f;
		else if (key == Common::KEY_3)
			SoftBody::WIND.x -= 0.1f;
		else if (key == Common::KEY_4)
			SoftBody::WIND.y += 0.1f;
		else if (key == Common::KEY_5)
			SoftBody::WIND.y = 0.0f;
		else if (key == Common::KEY_6)
			SoftBody::WIND.y -= 0.1f;
		else if (key == Common::KEY_7)
			SoftBody::WIND.z += 0.1f;
		else if (key == Common::KEY_8)
			SoftBody::WIND.z = 0.0f;
		else if (key == Common::KEY_9)
			SoftBody::WIND.z -= 0.1f;
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

		if (m_animate)
			m_ballAnimator->update(dt);

		if (!m_started)
			return;

		if (g_body) {
			g_body->setConstrained(0, 0, m_constrained);
			g_body->setConstrained(SoftBody::WIDTH-1, 0, m_constrained);
		}

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

		if (m_debug)
		{
			DebugIterator dit = m_debugComponents.find(m_currentLevel);
			if (dit != m_debugComponents.end())
			{
				ComponentCollection components = dit->second;
				for (ComponentIterator it = components.begin(); it != components.end(); it++)
				{
					(*it)->draw();
				}
			}
		}

		std::stringstream ss;
		ss << "Cloth Simulation Demo" << std::endl;
		ss << std::setiosflags(std::ios::fixed) << std::setprecision(1);
		ss << std::endl;
		
		if (m_help) {
			ss << "<Space> Start/reset simulation" << std::endl;
			ss << "<Enter> Toggle BVH visualization" << std::endl;
			ss << "<+/-> Adjust BVH visualization level";
			if (m_debug) { ss << " (" << m_currentLevel << ")"; } ss << std::endl;
			ss << "<c> Toggle constrained corners: " << (m_constrained ? "true" : "false") << std::endl;
			ss << "<v> Toggle moving ball: " << (m_animate ? "true" : "false") << std::endl;
			ss << "<Numpad 1-9> Adjust wind speed" << std::endl;
			ss << "<h> Show this help (reduces fps)" << std::endl;
			ss << std::endl;
			ss << "Friction: " << SoftBody::FRICTION << std::endl;
			ss << "Iteration count: " << SoftBody::ITERATION_COUNT << std::endl;
			ss << "Cloth size: " << SoftBody::WIDTH << "x" << SoftBody::LENGTH << std::endl;
		}
		ss << "Wind: (" << SoftBody::WIND.x << ", " << SoftBody::WIND.y << ", " << SoftBody::WIND.z << ")" << std::endl;

		std::string text = ss.str();

		display_text(text.c_str(), 10, 15);

		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			Trace::error("OpenGL error: %d\n", err);
	}
}
