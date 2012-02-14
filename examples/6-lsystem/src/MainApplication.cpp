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

#include "LSystem.h"
#include "MainApplication.h"
#include "Material.h"
#include "MeshObject.h"
#include "Trace.h"
#include "TurtleRenderer.h"
#include "Utils.h"

#include <GL/glew.h>

#include <sstream>
#include <iomanip>

namespace LSystemDemo
{
	using namespace Common;

	static Utils::Random random;

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

		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClearDepth(1.0f);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_ALPHA_TEST);

		glAlphaFunc(GL_GREATER, 0.5f);

		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		Common::GameObject::s_camera = &m_camera;
		MeshFactory::setStaticDraw(true);

		initScene();
		
		return true;
	}

	void MainApplication::initScene()
	{
		m_components.clear();

		Material * grassMaterial = new Material(Shader::find("shader"));
		grassMaterial->setTexture(new Texture("resources/ground.bmp"));
		grassMaterial->setDiffuseColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		grassMaterial->setAmbientColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

		Mesh * terrain = MeshFactory::Terrain();
		MeshObject * grass = new MeshObject(terrain, grassMaterial);
		grass->transform().scale(glm::vec3(10.0f));
		grass->transform().translate(glm::vec3(-5.0f, 0.0f, -5.0f));

		m_components.push_back(grass);

		{ // palm tree
			PlantDefinition * def = new PlantDefinition();
			def->iterations = 2;
			def->angle = 22.5f;
			def->diameter = 0.10f;
			def->length = 0.20f;
			def->thinning = 1.9f;
			def->axiom = "F";
			def->addTerminals("+-!?&/[]Q");
			def->addProduction('F', "F[-F]F[+F]F[&F]F[/F]F[QQQQQ]F");
			def->barkTexture = "resources/bark_palm.bmp";
			def->leafTexture = "resources/palm_leaf.bmp";
			def->size = 0.5f;

			LSystem * system = new LSystem(def);

			glm::vec3 position = terrain->vertexAt(random.randXX(0, terrain->vertices().size()-1)).position;
			position = glm::vec3(grass->transform().world() * glm::vec4(position, 1.0f));
			system->transform().translate(position);
			m_components.push_back(system);
		}

		{ // tropical tree
			PlantDefinition * def = new PlantDefinition();
			def->addTerminals("+-!?&/[]<Q`");
			def->iterations = 7;
			def->angle = 22.5f;
			def->diameter = 0.15f;
			def->length = 0.15f;
			def->thinning = 1.5f;
			def->axiom = "A";
			def->addProduction('A', "[+FLA]?????[+FLA]???????`[+FLA]");
			def->addProduction('F', "S?????F");
			def->addProduction('S', "F");
			def->addProduction('L', "[Q--Q][Q&&Q]");
			def->barkTexture = "resources/bark.bmp";
			def->leafTexture = "resources/leaf.bmp";

			LSystem * system = new LSystem(def);

			glm::vec3 position = terrain->vertexAt(random.randXX(0, terrain->vertices().size()-1)).position;
			position = glm::vec3(grass->transform().world() * glm::vec4(position, 1.0f));
			system->transform().translate(position);
			m_components.push_back(system);
		}

		{ // small tree
			PlantDefinition * def = new PlantDefinition();
			def->addTerminals("+-!?&/[]<FQ");
			def->iterations = 13;
			def->angle = 15.0f;
			def->diameter = 0.02f;
			def->length = 0.15f;
			def->thinning = 1.3f;
			def->axiom = "FA";
			def->addProduction('A', "/FB???B?????BQ");
			def->addProduction('B', "[//F??????A]");
			def->barkTexture = "resources/bark2.bmp";
			def->leafTexture = "resources/papaya_leaf.bmp";

			LSystem * system = new LSystem(def, 2000.0f);
		
			glm::vec3 position = terrain->vertexAt(random.randXX(0, terrain->vertices().size()-1)).position;
			position = glm::vec3(grass->transform().world() * glm::vec4(position, 1.0f));
			system->transform().translate(position);

			m_components.push_back(system);
		}

		{ // "maple" tree
			PlantDefinition * def = new PlantDefinition();
			def->addTerminals("+-!?&/[]<FQ");
			def->iterations = 6;
			def->angle = 19.0f;
			def->diameter = 0.075f;
			def->length = 0.20f;
			def->thinning = 2.0f;
			def->axiom = "F&A";
			def->addProduction('A', "F[-F/AQ]?[+FAQ]&[!FAQ]");
			def->barkTexture = "resources/bark_maple.bmp";
			def->leafTexture = "resources/maple_leaf.bmp";

			LSystem * system = new LSystem(def, 6000.0f);
		
			glm::vec3 position = terrain->vertexAt(random.randXX(0, terrain->vertices().size()-1)).position;
			position = glm::vec3(grass->transform().world() * glm::vec4(position, 1.0f));
			system->transform().translate(position);

			m_components.push_back(system);
		}

		{ // deathly tree
			PlantDefinition * def = new PlantDefinition();
			def->addTerminals("+-!?&/[]Q");
			def->iterations = 3;
			def->angle = 22.5f;
			def->diameter = 0.02f;
			def->length = 0.20f;
			def->thinning = 1.1f;
			def->axiom = "F";
			def->addProduction('F', "FF-[-F&F!F]QQ![&F??FQ-FQ]");
			def->barkTexture = "resources/bark_black.bmp";
			def->leafTexture = "resources/white_leaf.bmp";

			LSystem * system = new LSystem(def, 3500.0f);
		
			glm::vec3 position = terrain->vertexAt(random.randXX(0, terrain->vertices().size()-1)).position;
			position = glm::vec3(grass->transform().world() * glm::vec4(position, 1.0f));
			system->transform().translate(position);

			m_components.push_back(system);
		}

		//Trace::info("Generated: %s\n", plant->generate().c_str());
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
			m_started = !m_started;
		else if (key == Common::KEY_RESET_1)
			initScene();
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
	}

	void MainApplication::draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (ComponentIterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			(*it)->draw();
		}

		if (m_started) {
			GLenum err = glGetError();
			if (err != GL_NO_ERROR)
				Trace::error("OpenGL error: %d\n", err);
		}

		std::stringstream ss;
		ss << "Procedural Modeling Demo" << std::endl;
		ss << std::setiosflags(std::ios::fixed) << std::setprecision(1);
		ss << std::endl;
		ss << "Trees generated by context-free L-Systems" << std::endl;
		ss << "Fractal terrain generated by Diamond-Square algorithm" << std::endl;
		ss << std::endl;
		ss << "<Space> Start/pause simulation" << std::endl;
		ss << "<Enter> Reset simulation" << std::endl;

		std::string text = ss.str();

		display_text(text.c_str(), 10, 15);
	}
}
