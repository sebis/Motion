#include "GameObject.h"
#include "TurtleRenderer.h"

#include <stack>
#include <iostream>

namespace Common
{
	TurtleRenderer::TurtleRenderer(LSystem * system)
		: Renderer(0), m_system(system)
	{
		m_material = new Material(Shader::find("shader"));
		m_material->setDiffuseColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_mesh = MeshFactory::Cylinder(15, glm::vec4(0.8f, 0.5f, 0.25f, 1.0f));

		m_leafMaterial = new Material(Shader::find("shader"));
		m_leafMaterial->setTexture(new Texture("resources/leaf.bmp"));

		m_leafMesh = MeshFactory::Plane();
	}

	TurtleRenderer::~TurtleRenderer()
	{
	}

	// TODO: temp here
	namespace
	{
		float cost = 20.0f;
		float length = 0.25f;
		float r = 1.4f;
		float thickness = 0.1f;

		float growth(float min, float max, float T0, float T, float t)
		{
			if (t < T0)
				return min;
			if (t > T)
				return max;

			float d = max - min;
			return -2*(d/(T*T*T))*t*t*t + 3*(d/(T*T))*t*t + min;
		}
	}

	void TurtleRenderer::drawLeaf()
	{
		const glm::mat4 & m = m_stack.top();
	
		glm::mat4 temp = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(length)), glm::vec3(0.0f, 1.0f, 0.0f));

		//m_leafMaterial->shader()->setUniform("world", m * temp);
		//m_leafMesh->draw();
		m_leaves.push_back(m * temp);
	}

	void TurtleRenderer::drawSegment(int level)
	{
		const glm::mat4 & m = m_stack.top();
	
		glm::mat4 s = glm::scale(m, glm::vec3(thickness, 0.5f*length, thickness));
		glm::mat4 t = glm::translate(s, glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 temp = glm::translate(glm::scale(glm::mat4(m), glm::vec3(thickness, 0.5f*length, thickness)), glm::vec3(0.0f, 1.0f, 0.0f));

		//m_material->shader()->setUniform("world", m * temp);
		//m_mesh->draw();
		m_branches.push_back(t);
	}

	void TurtleRenderer::parseSystem()
	{
		m_stack = std::stack<glm::mat4>();
		m_stack.push(glm::mat4(1.0f));
		int level = 1;

		// TODO: this call is temp
		;
		std::string path = m_system->generate();

		float angle = m_system->definition()->angle;

		glm::mat4 m, top;

		std::stack<std::string> queue = m_system->getQueue();
		int generations = queue.size();
		//Trace::info("generations: %d\n", generations);

		//length = 0.0f + (1.0f - 0.0f) / (1 + std::expf(0.0001f*(5000.0f - m_system->time())));
		/*for (unsigned gen = 0; gen < generations; gen++)
		{
			std::string str = queue.top();
			Trace::info("%s\n", str);
			queue.pop();
		}*/

		//for (unsigned gen = generations-1; gen >= 0; gen--)
		{
			//std::string path = queue.top();
			//queue.pop();

			//std::cout << "Rendering generation: " << gen << " -- " << path.length() << std::endl;;

			/*float gen_starttime = gen * 2000.0f;
			if (m_system->time() < gen_starttime)
				continue;*/

			//length = growth(0.0f, 1.0f, 10000.0f + gen_starttime, m_system->time());
			//thickness = growth(0.05f, 0.1f, 10000.0f + gen_starttime, m_system->time());

			int age = 0;
			float growTime = 20.0f;

			float modulesPerMSec = 1000.0f / growTime;
			float time = m_system->time();
			Trace::info("time: %f\n", time);

			int numModules = time / growTime;
			//Trace::info("numModules: %d\n", numModules);
			int mod = 0;

			for (unsigned i = 0; i < path.length(); i++)
			{
				
				/*float level_StartTime = level * growTime;
				if (m_system->time() < level_StartTime)
					continue;*/

				//length = growth(0.0f, 0.25f, level_StartTime, level_StartTime + 50000.0f, m_system->time());
				//thickness = growth(0.0f, 0.015f, (7-age) * 2000.0f, (6-age) * 2000.0f, m_system->time());

				char chr = path[i];
				switch (chr)
				{
				case 'k':
					age = std::atoi(&path[i+1]);
					i++;
					break;
				case 'X':
					// ignored symbol
					break;
				case 'F':
					//if (mod++ < numModules)
					if (time < 0.0f)
						break;
					drawSegment(level);
					time -= cost;
					m = glm::translate(m_stack.top(), glm::vec3(0.0f, length, 0.0f));
					m_stack.pop();
					m_stack.push(m);
					break;
				case 'Q':
					drawLeaf();

					break;
				case '+':
					m = glm::rotate(m_stack.top(), angle, glm::vec3(1.0f, 0.0f, 0.0f));
					m_stack.pop();
					m_stack.push(m);
					break;
				case '-':
					m = glm::rotate(m_stack.top(), -angle, glm::vec3(1.0f, 0.0f, 0.0f));
					m_stack.pop();
					m_stack.push(m);
					break;
				case '!':
					m = glm::rotate(m_stack.top(), angle, glm::vec3(0.0f, 1.0f, 0.0f));
					m_stack.pop();
					m_stack.push(m);
				case '?':
					m = glm::rotate(m_stack.top(), -angle, glm::vec3(0.0f, 1.0f, 0.0f));
					m_stack.pop();
					m_stack.push(m);
				case '&':
					m = glm::rotate(m_stack.top(), angle, glm::vec3(0.0f, 0.0f, 1.0f));
					m_stack.pop();
					m_stack.push(m);
					break;
				case '/':
					m = glm::rotate(m_stack.top(), -angle, glm::vec3(0.0f, 0.0f, 1.0f));
					m_stack.pop();
					m_stack.push(m);
					break;
				case '[':
					m_stack.push(m_stack.top());
					thickness /= r;
					level++;
					break;
				case ']':
					m_stack.pop();
					thickness *= r;
					level--;
					break;
				default:
					{
					//Trace::info("Encountered unkown turtle command: %c\n", chr);
					}
				}
			}
			queue.pop();
			//break;
		}
	}

	void TurtleRenderer::draw()
	{
		m_leaves.clear();
		m_branches.clear();

		parseSystem();

		glDisable(GL_BLEND);

		m_material->begin();

		m_material->shader()->setUniform("view", GameObject::s_camera->view());
		m_material->shader()->setUniform("projection", GameObject::s_camera->projection());

		for (unsigned i = 0; i < m_branches.size(); i++) {
			m_material->shader()->setUniform("world", m_branches[i]);
			m_mesh->draw();
		}

		m_material->end();

		glEnable(GL_BLEND);

		m_leafMaterial->begin();

		m_leafMaterial->shader()->setUniform("view", GameObject::s_camera->view());
		m_leafMaterial->shader()->setUniform("projection", GameObject::s_camera->projection());

		for (unsigned i = 0; i < m_leaves.size(); i++) {
			m_leafMaterial->shader()->setUniform("world", m_leaves[i]);
			m_leafMesh->draw();
		}

		m_leafMaterial->end();
	}
}
