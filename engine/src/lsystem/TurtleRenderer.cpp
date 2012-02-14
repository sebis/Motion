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

#include "GameObject.h"
#include "TurtleRenderer.h"
#include "Utils.h"

#include <algorithm>
#include <queue>
#include <iostream>

namespace Common
{
	// TODO: temp here
	namespace
	{
		float length = 0.1f;
		float diameter = 0.1f;
		float thinning = 1.0f;
		float size = 0.15f;

		float growTime = 200.0f;
		float thickness = 1.0f;

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

	TurtleRenderer::TurtleRenderer(LSystem * system)
		: Renderer(system), m_system(system), m_root(0)
	{
		diameter = system->definition()->diameter;
		length = system->definition()->length;
		thinning = system->definition()->thinning;
		size = system->definition()->size;
		growTime = system->definition()->growtime;

		m_material = new Material(Shader::find("shader"));
		m_material->setTexture(new Texture(system->definition()->barkTexture.c_str()));
		m_material->setAmbientColor(glm::vec4(0.6f, 0.5f, 0.5f, 1.0f));
		m_material->setDiffuseColor(glm::vec4(0.6f, 0.4f, 0.5f, 1.0f));
		m_material->setSpecularColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		m_mesh = MeshFactory::Cylinder(15, system->definition()->diameter);

		m_leafMaterial = new Material(Shader::find("shader"));
		m_leafMaterial->setTexture(new Texture(system->definition()->leafTexture.c_str()));
		m_leafMaterial->setAmbientColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
		m_leafMaterial->setDiffuseColor(glm::vec4(0.6, 0.6, 0.6, 1.0f));
		m_leafMaterial->setSpecularColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		m_leafMesh = MeshFactory::Plane();

		parseSystem();
	}

	TurtleRenderer::~TurtleRenderer()
	{
	}

	TurtleRenderer::Node * TurtleRenderer::drawLeaf(Node * parent)
	{
		static Utils::Random rand;

		assert(parent);

		const glm::mat4 & m = m_stack.top();
	
		glm::mat4 t = glm::translate(m, glm::vec3(0.0f, length, 0.0f));
		glm::mat4 r = glm::rotate(t, float(rand.rand11()) * 180.0f, glm::vec3(float(rand.rand01()), float(rand.rand01()), float(rand.rand01())));
		glm::mat4 s = glm::scale(r, glm::vec3(size));

		Node * node = new Node(s);
		node->isLeaf = true;
		node->parent = parent;

		parent->children.push_back(node);

		return node;
	}

	TurtleRenderer::Node * TurtleRenderer::drawSegment(Node * parent)
	{
		const glm::mat4 & m = m_stack.top();
	
		assert(parent);

		glm::mat4 s = glm::scale(m, glm::vec3(thickness, length, thickness));

		Node * node = new Node(s);
		node->parent = parent;

		parent->children.push_back(node);

		return node;
	}

	void TurtleRenderer::parseSystem()
	{
		m_stack = std::stack<glm::mat4>();
		m_stack.push(glm::mat4(1.0f));
		
		std::stack<Node*> branchStack;

		std::string path = m_system->generate();
		//std::cout << "Word: " << path << std::endl;

		float angle = m_system->definition()->angle;

		glm::mat4 m;
		glm::vec3 right, up, forward;
		Node * parent = m_root = new Node();

		for (unsigned i = 0; i < path.length(); i++)
		{
			char chr = path[i];
			switch (chr)
			{
			case 'X':
				// ignored symbol
				break;
			case 'F':
				parent = drawSegment(parent);
				m = glm::translate(m_stack.top(), glm::vec3(0.0f, length, 0.0f));
				m_stack.pop();
				m_stack.push(m);
				break;
			case 'Q':
			case 'f':
				parent = drawLeaf(parent);
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
			case '|':
				m = glm::rotate(m_stack.top(), 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
				m_stack.pop();
				m_stack.push(m);
				break;
			case '[':
				m_stack.push(m_stack.top());
				thickness /= thinning;
				branchStack.push(parent);
				break;
			case ']':
				m_stack.pop();
				thickness *= thinning;
				parent = branchStack.top();
				branchStack.pop();
				break;
			default:
				{
				//Trace::info("Encountered unkown turtle command: %c\n", chr);
				}
			}
		}
	}

	void TurtleRenderer::draw()
	{
		if (!m_root)
			return;

		float time = m_system->time();
		if (time <= 0.0f)
			return;

		const glm::mat4 & world = glm::scale(m_gameObject->transform().worldMatrix(), glm::vec3(std::min(1.0f, time / 2000.0f)));

		glDisable(GL_BLEND);

		m_material->begin();

		m_material->shader()->setUniform("view", GameObject::s_camera->view());
		m_material->shader()->setUniform("projection", GameObject::s_camera->projection());

		Node * node = 0;

		std::queue<Node*> queue;
		std::vector<Node*> leaves;

		for (unsigned i = 0; i < m_root->children.size(); i++)
		{
			queue.push(m_root->children[i]);
		}

		while (!queue.empty())
		{
			node = queue.front();
			queue.pop();

			if (node->state == INIT) {
				node->birthTime = time;
				node->state = GROWING;
				continue;
			}

			if (node->state == GROWING) {
				node->scale = (time - node->birthTime) / growTime;
				if (node->scale > 1.0f) {
					node->scale = 1.0f;
					node->state = FINISHED;
				}
			}

			if (node->isLeaf) {
				leaves.push_back(node);
			} else {
				const glm::mat4 & m = node->m;
				m_material->shader()->setUniform("world", world * glm::scale(m, glm::vec3(1.0f, node->scale, 1.0f)));
				m_mesh->draw();
			}

			if (node->state == FINISHED || node->isLeaf)
			{
				for (unsigned i = 0; i < node->children.size(); i++)
				{
					queue.push(node->children[i]);
				}
			}
		}

		m_material->end();

		m_leafMaterial->begin();

		m_leafMaterial->shader()->setUniform("view", GameObject::s_camera->view());
		m_leafMaterial->shader()->setUniform("projection", GameObject::s_camera->projection());

		for (std::vector<Node*>::const_iterator it = leaves.begin(); it != leaves.end(); it++)
		{
			node = *it;

			const glm::mat4 & m = node->m;

			m_leafMaterial->shader()->setUniform("world", world * glm::scale(m, glm::vec3(node->scale)));
			m_leafMesh->draw();
		}

		m_leafMaterial->end();

		glEnable(GL_BLEND);
	}
}
