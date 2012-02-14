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

#ifndef COMMON_LINERENDERER
#define COMMON_LINERENDERER

#include "Renderer.h"
#include "Shader.h"
#include "Utils.h"

#include "glm/glm.hpp"

#include <vector>

namespace Common
{
	class LineRenderer : public Renderer
	{
	public:
		struct vertex {
			glm::vec3 position;
			glm::vec4 color;
		};

		LineRenderer(GameObject * gameObject);

		virtual ~LineRenderer();

		inline void addLine(const vertex & a, const vertex & b)
		{
			m_vertices.push_back(a);
			m_vertices.push_back(b);
		}

		inline void clear()
		{
			m_vertices.clear();
		}

		virtual void draw();

	private:
		std::vector<vertex> m_vertices;

		Shader * m_shader;

		glm::uint m_vaoID;
		glm::uint m_vboID;
	};
}

#endif /* COMMON_LINERENDERER */