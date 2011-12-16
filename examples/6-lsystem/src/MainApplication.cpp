#include "LSystem.h"
#include "MainApplication.h"
#include "Material.h"
#include "MeshObject.h"
#include "Trace.h"
#include "TurtleRenderer.h"
#include "Utils.h"

#include <GL/glew.h>

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

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClearDepth(1.0f);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_ALPHA_TEST);

		glAlphaFunc(GL_GREATER, 0.1f);

		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		Common::GameObject::s_camera = &m_camera;
		MeshFactory::setStaticDraw(true);

		Material * grassMaterial = new Material(Shader::find("shader"));
		grassMaterial->setTexture(new Texture("resources/grass.bmp"));
		grassMaterial->setDiffuseColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		grassMaterial->setAmbientColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));

		Mesh * terrain = MeshFactory::Terrain();
		MeshObject * grass = new MeshObject(terrain, grassMaterial);
		grass->transform().scale(glm::vec3(30.0f));
		grass->transform().translate(glm::vec3(-15.0f, 0.0f, -15.0f));

		m_components.push_back(grass);

		// For testing purposes
		/*PlantDefinition * def = new PlantDefinition();
		def->iterations = 2;
		def->axiom = "F";
		def->addTerminals("+-!?&/[]");
		//def->addProduction('F', "F[-F]F[+F]F");
		def->addProduction('F', "F[-F]F[+F]F[&F]F[/F]FF");*/

		/*PlantDefinition * def = new PlantDefinition();
		def->addTerminals("+-[]");
		def->iterations = 6;
		def->axiom = "X";
		def->addProduction('X', "F-[[X]+X]+F[+FX]-X");
		def->addProduction('F', "FF", 5000.0f);
		def->angle = glm::vec3(25.0f, 0.0f, 0.0f);*/

		// tropical tree
		PlantDefinition * treeDef = new PlantDefinition();
		treeDef->addTerminals("+-!?&/[]<Q`");
		treeDef->iterations = 7;
		treeDef->angle = 22.5f;
		treeDef->diameter = 0.15f;
		treeDef->length = 0.15f;
		treeDef->thinning = 1.5f;
		treeDef->axiom = "A";
		treeDef->addProduction('A', "[+FLA]?????[+FLA]???????`[+FLA]");
		treeDef->addProduction('F', "S?????F");
		treeDef->addProduction('S', "F");
		treeDef->addProduction('L', "[Q--Q][Q&&Q]");
		//treeDef->addProduction('L', "Q");

		LSystem * tree = new LSystem(treeDef);

		glm::vec3 position = terrain->vertexAt(random.randXX(0, terrain->vertices().size()-1)).position;
		position = glm::vec3(grass->transform().world() * glm::vec4(position, 1.0f));
		tree->transform().translate(position);
		m_components.push_back(tree);

		PlantDefinition * treeDef2 = new PlantDefinition();
		treeDef2->addTerminals("+-!?&/[]<FQ");
		treeDef2->iterations = 13;
		treeDef2->angle = 15.0f;
		treeDef2->diameter = 0.02f;
		treeDef2->length = 0.15f;
		treeDef2->thinning = 1.3f;
		treeDef2->axiom = "FA";
		treeDef2->addProduction('A', "/FB???B?????BQ");
		treeDef2->addProduction('B', "[//F??????A]");

		LSystem * tree2 = new LSystem(treeDef2);
		
		position = terrain->vertexAt(random.randXX(0, terrain->vertices().size()-1)).position;
		position = glm::vec3(grass->transform().world() * glm::vec4(position, 1.0f));
		tree2->transform().translate(position);

		m_components.push_back(tree2);

		// bush-like structure
		/*PlantDefinition * testDef = new PlantDefinition();
		testDef->addTerminals("+-!?&/[]kFQ");
		testDef->iterations = 3;
		testDef->angle = 30.0f;
		testDef->axiom = "A";
		//testDef->addProduction('F', "FF[+FF][-FF]");
		testDef->addProduction('A', "[+FAQ]?[+FAQ]");

		LSystem * test = new LSystem(testDef);
		m_components.push_back(test);
		m_components.push_back(new TurtleRenderer(test));*/

		//Trace::info("Generated: %s\n", plant->generate().c_str());
		
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
		else if (key == Common::KEY_CONTINUE)
			m_started = !m_started;
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
	}
}
