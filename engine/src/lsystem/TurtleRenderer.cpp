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
		m_mesh = MeshFactory::Cube(false, glm::vec4(0.8f, 0.5f, 0.25f, 1.0f));
	}

	TurtleRenderer::~TurtleRenderer()
	{
	}

	// TODO: temp here
	namespace
	{
		float length = 1.0f;
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

	void TurtleRenderer::drawSegment(int level)
	{
		const glm::mat4 & m = m_stack.top();
	
		glm::mat4 temp = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(thickness, 0.5f*length, thickness)), glm::vec3(0.0f, 1.0f, 0.0f));

		m_material->shader()->setUniform("world", m * temp);

		m_mesh->draw();
	}

	void TurtleRenderer::parseSystem()
	{
		m_stack = std::stack<glm::mat4>();
		m_stack.push(glm::mat4(1.0f));
		int level = 1;

		// TODO: this call is temp
		m_system->generate();
		std::string path = m_system->get();

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

			float growTime = 20.0f;

			float modulesPerMSec = 1000.0f / growTime;
			float time = m_system->time();

			if (time <= 0)
				return;

			int numModules = time / growTime;
			Trace::info("numModules: %d\n", numModules);
			int mod = 0;

			for (unsigned i = 0; i < path.length(); i++)
			{
				float level_StartTime = level * growTime;
				if (m_system->time() < level_StartTime)
					continue;

				length = growth(0.0f, 0.25f, level_StartTime, level_StartTime + 50000.0f, m_system->time());
				thickness = growth(0.005f, 0.015f, level_StartTime, 10000.0f, m_system->time());

				char chr = path[i];
				switch (chr)
				{
				case 'X':
					// ignored symbol
					break;
				case 'F':
					if (mod++ < numModules)
						drawSegment(level);
					m = glm::translate(m_stack.top(), glm::vec3(0.0f, length, 0.0f));
					m_stack.pop();
					m_stack.push(m);
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
		glDisable(GL_BLEND);
		m_material->begin();

		m_material->shader()->setUniform("view", GameObject::s_camera->view());
		m_material->shader()->setUniform("projection", GameObject::s_camera->projection());

		parseSystem();

		m_material->end();
		glEnable(GL_BLEND);
	}
}
