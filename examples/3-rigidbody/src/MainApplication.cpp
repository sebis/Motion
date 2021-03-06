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

#include "glm/gtc/matrix_transform.hpp"

#include <cmath>
#include <sstream>
#include <iomanip>
#include <GL/glew.h>

namespace RigidBodyDemo
{
	using namespace Common;
	using namespace Interpolation;

	MainApplication::MainApplication(const char * title, bool fixedTimeStep, float targetElapsedTime)
		: Base(title, fixedTimeStep, targetElapsedTime),
		m_camera(glm::vec3(10.0f), glm::vec3(0.0f)), m_visualize(false), m_hideHelp(false)
	{
	}

	MainApplication::~MainApplication()
	{
		delete m_physics;
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

		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClearDepth(1.0f);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		glLineWidth(0.7f);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		Common::GameObject::s_camera = &m_camera;

		m_physics = new Physics();

		setupSimulation();
		
		return true;
	}

	void MainApplication::reset()
	{
		delete m_physics;
		CollisionDetector::instance()->clear();
		m_components.clear();

		m_physics = new Physics();
		setupSimulation();
	}

	void MainApplication::setupSimulation()
	{
		m_started = false;

		/// Create floor

		Material * floorMaterial = new Material(Shader::find("shader"));
		floorMaterial->setTexture(new Texture("resources/floor.bmp"));
		floorMaterial->setAmbientColor(glm::vec4(0.3f));
		floorMaterial->setShininess(120.0f);
		floorMaterial->setDiffuseColor(glm::vec4(0.6f));
		floorMaterial->setSpecularColor(glm::vec4(0.2f));

		MeshObject * floor = new MeshObject(MeshFactory::Plane(glm::vec4(1.0f), 5), floorMaterial);
		floor->transform().scale() = glm::vec3(5.0f, 1.0f, 5.0f);

		PlaneCollider * floorCollider = new PlaneCollider(floor, 0);
		floorCollider->m_normal = glm::vec3(0.0f, 1.0f, 0.0f);
		floorCollider->m_d = 0.0f;
		floorCollider->name = "floor";
		CollisionDetector::instance()->addCollider(floorCollider);

		m_components.push_back(floor);

		// Create pool table

		Material * tableMaterial = new Material(Shader::find("shader"));
		tableMaterial->setDiffuseColor(glm::vec4(0.8f));
		tableMaterial->setAmbientColor(glm::vec4(0.2f));
		tableMaterial->setSpecularColor(glm::vec4(0.3f));
		tableMaterial->setTexture(new Texture("resources/table.bmp"));

		MeshObject * table = new MeshObject(MeshFactory::FromFile("resources/pooltable.ply"), tableMaterial);
		//table->transform().scale() = glm::vec3(0.01f);
		m_components.push_back(table);

		// Create some walls

		PlaneCollider * tabletop = new PlaneCollider(0);
		tabletop->m_normal = glm::vec3(0.0f, 1.0f, 0.0f);
		tabletop->m_d = 1.2f;
		tabletop->name = "tabletop";
		CollisionDetector::instance()->addCollider(tabletop);

		PlaneCollider * wall1 = new PlaneCollider(0);
		wall1->m_normal = glm::vec3(-1.0f, 0.0f, 0.0f);
		wall1->m_d = -0.939f;
		wall1->name = "wall1";
		CollisionDetector::instance()->addCollider(wall1);

		PlaneCollider * wall2 = new PlaneCollider(0);
		wall2->m_normal = glm::vec3(1.0f, 0.0f, 0.0f);
		wall2->m_d = -0.920f;
		wall2->name = "wall2";
		CollisionDetector::instance()->addCollider(wall2);

		PlaneCollider * wall3 = new PlaneCollider(0);
		wall3->m_normal = glm::vec3(0.0f, 0.0f, -1.0f);
		wall3->m_d = -1.812f;
		wall3->name = "wall3";
		CollisionDetector::instance()->addCollider(wall3);

		PlaneCollider * wall4 = new PlaneCollider(0);
		wall4->m_normal = glm::vec3(0.0f, 0.0f, 1.0f);
		wall4->m_d = -1.839f;
		wall4->name = "wall4";
		CollisionDetector::instance()->addCollider(wall4);

		/// Create cube

		int count = 15;
		float radius = 0.025f;
		float omega = float(2*M_PI) / count;

		for (int i = 0; i < count; i++)
		{
			Material * cubeMaterial = new Material(Shader::find("shader"));
			std::stringstream ss;
			ss << "resources/ball-" << i << ".bmp";
			cubeMaterial->setTexture(new Texture(ss.str().c_str()));
			cubeMaterial->setDiffuseColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

			MeshObject * cube = new MeshObject(MeshFactory::Sphere(), cubeMaterial);
			//cube->transform().translate(glm::vec3(i * 0.05f, 0.2f + 2 * float(i / 20.0f), i * 0.025f));
			cube->transform().translate(glm::vec3(radius * std::cos(i*omega), 1.25f + i * 0.25f, radius * std::sin(i*omega)));
			cube->transform().scale() = glm::vec3(0.05715f);
			cube->m_rigidbody = new RigidBody(cube);

			SphereCollider * cubeCollider = new SphereCollider(cube, cube->m_rigidbody);
			cubeCollider->m_radius = 0.05715f;
			ss.clear();
			ss << "sphere" << i;
			cubeCollider->name = ss.str();
			CollisionDetector::instance()->addCollider(cubeCollider);

			m_components.push_back(cube);

			m_physics->addObject(cube->m_rigidbody);
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
			if (m_started)
				reset();
			else
				m_started = true;
		}
		else if (key == Common::KEY_HELP)
			m_hideHelp = !m_hideHelp;
		else if (key == Common::KEY_RESET_1)
			m_physics->explode(glm::vec3(0.0f));
		else if (key == Common::KEY_RESET_2)
			m_visualize = !m_visualize;
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

		m_physics->step(dt);

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

		if (m_visualize)
			m_physics->visualize();

		std::stringstream ss;
		ss << std::setiosflags(std::ios::fixed) << std::setprecision(2);
		ss << "Rigid body dynamics demo" << std::endl;
		if (!m_hideHelp)
		{
			ss << "Simulation consists of sphere colliders and plane colliders." << std::endl;
			ss << "Plane colliders are infinite, ie. balls cannot fly over the rails." << std::endl;
			ss << std::endl;
			ss << "<Space> - start/reset simulation" << std::endl;
			ss << "<Enter> - Apply impulse" << std::endl;
			ss << "<c> - Toggle visualisation" << std::endl;
			ss << "<h> - Toggle this help" << std::endl;
			ss << std::endl;
			ss << "Coefficient of restitution (ball-ball): " << COF_BALL_BALL << std::endl;
			ss << "Coefficient of restitution (ball-rail): " << COF_BALL_RAIL << std::endl;
			ss << "Coefficient of restitution (ball-table): " << COF_BALL_TABLE << std::endl;
			ss << "Friction (ball-ball): " << FRICTION_BALL_BALL << std::endl;
			ss << "Friction (ball-cloth): " << FRICTION_BALL_CLOTH << std::endl;
		}
		ss << std::endl;
		ss << "Total energy: " << m_physics->calculateEnergy(1.2f + 0.05715f) << " J" << std::endl;

		display_text(ss.str().c_str(), 10, 15);
	}
}
