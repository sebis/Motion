#include "GameObject.h"
#include "TurtleRenderer.h"
#include "Utils.h"

#include <algorithm>
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

		parseSystem();
	}

	TurtleRenderer::~TurtleRenderer()
	{
	}

	// TODO: temp here
	namespace
	{
		float cost = 200.0f;
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

	void TurtleRenderer::drawLeaf(int level, int branch)
	{
		const glm::mat4 & m = m_stack.top();
	
		glm::mat4 temp = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(length)), glm::vec3(0.0f, 1.0f, 0.0f));

		//m_leafMaterial->shader()->setUniform("world", m * temp);
		//m_leafMesh->draw();
		Branches & branches = m_leaves[level];
		Branch & b = branches[branch];
		b.push_back(m * temp);
	}

	void TurtleRenderer::drawSegment(int level, int branch)
	{
		const glm::mat4 & m = m_stack.top();
	
		glm::mat4 s = glm::scale(m, glm::vec3(thickness, 0.5f*length, thickness));
		glm::mat4 t = glm::translate(s, glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 temp = glm::translate(glm::scale(glm::mat4(m), glm::vec3(thickness, 0.5f*length, thickness)), glm::vec3(0.0f, 1.0f, 0.0f));

		//m_material->shader()->setUniform("world", m * temp);
		//m_mesh->draw();
		Branches & branches = m_branches[level];
		Branch & b = branches[branch];
		b.push_back(t);
		Trace::info("Adding segment to level %d and branch %d -- size of branch: %d\n", level, branch, branches.size());
	}

	void TurtleRenderer::parseSystem()
	{
		m_stack = std::stack<glm::mat4>();
		std::stack<unsigned> branchStack;

		m_stack.push(glm::mat4(1.0f));
		branchStack.push(0);
		int level = 0;
		int branch = 0;
		int bracket = 0;

		// TODO: this call is temp
		;
		std::string path = m_system->generate();
		Trace::info("Word: %s\n", path.c_str());

		float angle = m_system->definition()->angle;

		glm::mat4 m, top;

		//std::stack<std::string> queue = m_system->getQueue();
		//int generations = queue.size();
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
					//age = std::atoi(&path[i+1]);
					//i++;
					break;
				case 'X':
					// ignored symbol
					break;
				case 'F':
					//if (mod++ < numModules)
					//if (time < 0.0f)
						//break;
					drawSegment(level, branch);
					//time -= cost;
					m = glm::translate(m_stack.top(), glm::vec3(0.0f, length, 0.0f));
					m_stack.pop();
					m_stack.push(m);
					break;
				case 'Q':
					drawLeaf(level, branch);

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
					break;
				case '?':
					m = glm::rotate(m_stack.top(), -angle, glm::vec3(0.0f, 1.0f, 0.0f));
					m_stack.pop();
					m_stack.push(m);
					break;
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
					//branch = branchStack.top();
					branchStack.push(branchStack.top()+1);
					branch++;
					bracket++;
					break;
				case ']':
					m_stack.pop();
					thickness *= r;
					level--;
					branchStack.pop();
					branch++;
					bracket--;
					if (bracket == 0)
						level++;
					break;
				default:
					{
					//Trace::info("Encountered unkown turtle command: %c\n", chr);
					}
				}
			}
			//queue.pop();
			//break;
		}

		for (Container::iterator it = m_branches.begin(); it != m_branches.end(); it++)
		{
			Branches & b = it->second;

			//std::random_shuffle(b.begin(), b.end());
		}
	}

	void TurtleRenderer::draw()
	{
		Utils::Random r(12);

		glDisable(GL_BLEND);

		m_material->begin();

		m_material->shader()->setUniform("view", GameObject::s_camera->view());
		m_material->shader()->setUniform("projection", GameObject::s_camera->projection());

		float time = m_system->time();
		Trace::info("Time: %f\n", time);

 		for (Container::iterator it = m_branches.begin(); it != m_branches.end(); it++)
		{

			// get branches for current level
			Branches & branches = it->second;

			if (time < 0.0f)
				break;

			for (Branches::iterator bit = branches.begin(); bit != branches.end(); bit++)
			{

				// select random branch
				//Branches::iterator bit = branches.begin();
				//std::advance(bit, r.randXX(0, branches.size()-1));
				Trace::info("branch size: %d\n", branches.size());
				Branch & b = bit->second;

				//assert(branches.size() == 1);

				for (int i = 0; i < b.size(); i++) {
				/*if (b.empty()) {
					branches.erase(bit);
					break;
				}*/
				if (time < 0.0f)
					break;

				// take first item from the branch for drawing
				//const glm::mat4 & m = b.front();
				const glm::mat4 & m = b[i];

				float scale = 1.0f;
				if (time < cost)
					scale = time / cost;
				time -= cost;

				
				Trace::info("Rendering level %d/%d and branch %d/%d\n", it->first, m_branches.size(), bit->first, b.size());

				m_material->shader()->setUniform("world", glm::scale(m, glm::vec3(1.0f, scale, 1.0f)));
				m_mesh->draw();

				//b.pop();
				}
			}
			

			/*for (int i = 0; i < branches.size(); i++)
			{
				if (time < 0.0f)
					break;

				float scale = 1.0f;
				if (time < cost)
					scale = time/cost;
				time -= cost;

				const glm::mat4 & m = branches[i];

				m_material->shader()->setUniform("world", glm::scale(m, glm::vec3(1.0f, scale, 1.0f)));
				m_mesh->draw();
			}*/
		}

		m_material->end();

		// for each level
			// select random branch
			// draw next item in branch

		/*glEnable(GL_BLEND);

		m_leafMaterial->begin();

		m_leafMaterial->shader()->setUniform("view", GameObject::s_camera->view());
		m_leafMaterial->shader()->setUniform("projection", GameObject::s_camera->projection());

		for (unsigned i = 0; i < m_leaves.size(); i++) {
			m_leafMaterial->shader()->setUniform("world", m_leaves[i]);
			m_leafMesh->draw();
		}

		m_leafMaterial->end();*/
	}
}
