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

#ifndef COMMON_TURTLERENDERER
#define COMMON_TURTLERENDERER

#include "MeshRenderer.h"
#include "LSystem.h"

#include <stack>
#include <vector>

namespace Common
{
	class TurtleRenderer : public Renderer
	{
	public:
		TurtleRenderer(LSystem * system);
		virtual ~TurtleRenderer();

		virtual void draw();

	private:

		enum State
		{
			INIT,
			GROWING,
			FINISHED
		};

		struct Node
		{
			Node(glm::mat4 _m = glm::mat4(1.0f)) : m(_m), state(INIT), birthTime(0.0f), scale(1.0f), parent(0), isLeaf(false) {}

			glm::mat4 m;

			State state;
			float birthTime;

			float scale;

			Node * parent;
			std::vector<Node*> children;

			bool isLeaf;
		};

		Node * drawLeaf(Node * parent);
		Node * drawSegment(Node * parent);
		void parseSystem();

		LSystem * m_system;

		Node * m_root;

		Material * m_material;
		Material * m_leafMaterial;
		Mesh * m_mesh;
		Mesh * m_leafMesh;

		std::stack<glm::mat4> m_stack;
	};
}

#endif /* COMMON_TURTLERENDERER */
