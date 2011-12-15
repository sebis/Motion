#include "GameObject.h"
#include "TurtleRenderer.h"
#include "Utils.h"

#include <algorithm>
#include <stack>
#include <iostream>

namespace Common
{
	TurtleRenderer::TurtleRenderer(LSystem * system)
		: Renderer(0), m_system(system), m_root(0)
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

	TurtleRenderer::Node * TurtleRenderer::drawLeaf(Node * parent)
	{
		assert(parent);

		const glm::mat4 & m = m_stack.top();
	
		glm::mat4 temp = glm::translate(glm::scale(m, glm::vec3(length)), glm::vec3(0.0f, 1.0f, 0.0f));

		Node * node = new Node(temp);
		node->mesh = m_leafMesh;
		node->parent = parent;

		parent->children.push_back(node);

		return node;
	}

	TurtleRenderer::Node * TurtleRenderer::drawSegment(Node * parent)
	{
		const glm::mat4 & m = m_stack.top();
	
		assert(parent);

		glm::mat4 s = glm::scale(m, glm::vec3(thickness, 0.5f*length, thickness));
		glm::mat4 t = glm::translate(s, glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 temp = glm::translate(glm::scale(glm::mat4(m), glm::vec3(thickness, 0.5f*length, thickness)), glm::vec3(0.0f, 1.0f, 0.0f));

		Node * node = new Node(t);
		node->mesh = m_mesh;
		node->parent = parent;

		parent->children.push_back(node);

		return node;
	}

	void TurtleRenderer::parseSystem()
	{
		m_stack = std::stack<glm::mat4>();
		m_stack.push(glm::mat4(1.0f));
		std::stack<unsigned> branchStack;
		
		std::stack<Node*> branchingStack;

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
		Node * parent = 0, * branching = 0;

		branching = parent = m_root = new Node();

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
				break;
			case 'X':
				// ignored symbol
				break;
			case 'F':
				//if (mod++ < numModules)
				//if (time < 0.0f)
					//break;
				parent = drawSegment(parent);
				//time -= cost;
				m = glm::translate(m_stack.top(), glm::vec3(0.0f, length, 0.0f));
				m_stack.pop();
				m_stack.push(m);
				break;
			case 'Q':
				//parent = drawLeaf(parent);

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
				branchingStack.push(parent);
				branching = parent;
				break;
			case ']':
				m_stack.pop();
				thickness *= r;
				level--;
				branchStack.pop();
				branch++;
				bracket--;
				//parent = parent->parent;
				parent = branchingStack.top();
				branchingStack.pop();

				if (bracket == 0)
					level++;
				break;
			default:
				{
				//Trace::info("Encountered unkown turtle command: %c\n", chr);
				}
			}
		}

		/*for (Container::iterator it = m_branches.begin(); it != m_branches.end(); it++)
		{
			Branches & b = it->second;

			//std::random_shuffle(b.begin(), b.end());
		}*/
	}

	void TurtleRenderer::draw()
	{
		if (!m_root)
			return;

		Utils::Random r(12);

		glDisable(GL_BLEND);

		m_material->begin();

		m_material->shader()->setUniform("view", GameObject::s_camera->view());
		m_material->shader()->setUniform("projection", GameObject::s_camera->projection());

		float time = m_system->time();
		Trace::info("Time: %f\n", time);

		Node * node = 0;

		std::queue<Node*> queue;

		for (int i = 0; i < m_root->children.size(); i++)
		{
			queue.push(m_root->children[i]);
		}

		while (!queue.empty())
		{
			if (time < 0.0f)
				break;

			node = queue.front();
			queue.pop();

			const glm::mat4 & m = node->m;
			m_material->shader()->setUniform("world", glm::scale(m, glm::vec3(1.0f, 1.0f, 1.0f)));
			node->mesh->draw();

			for (int i = 0; i < node->children.size(); i++)
			{
				float scale = 1.0f;
				if (time < cost)
					scale = time / cost;
				time -= cost;
				if (time < 0.0f)
					break;

				queue.push(node->children[i]);
			}
		}

		m_material->end();
	}
}
