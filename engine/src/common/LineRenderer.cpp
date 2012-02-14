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
#include "LineRenderer.h"

namespace Common
{
	LineRenderer::LineRenderer(GameObject * gameObject)
		: Renderer(gameObject)
	{
		m_shader = Shader::find("solid");

		// create single vertex array object and bind it
		glGenVertexArrays(1, &m_vaoID);

		glBindVertexArray(m_vaoID);

		// generate, bind and fill interleaved vertex buffer object with vertex attributes
		glGenBuffers(1, &m_vboID);

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		//glBufferData(GL_ARRAY_BUFFER, m_vertexCount*sizeof(vertex), &vData[0], GL_DYNAMIC_DRAW);

		// create vertex attribute pointers into our buffer data with correct strides and offsets
		glVertexAttribPointer(Shader::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
		glEnableVertexAttribArray(Shader::POSITION);
		glVertexAttribPointer(Shader::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, color));
		glEnableVertexAttribArray(Shader::COLOR);

		glBindVertexArray(0);
	}

	LineRenderer::~LineRenderer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_vboID);

		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vaoID);
	}

	void LineRenderer::draw()
	{
		glBindVertexArray(m_vaoID);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

		glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(vertex), &m_vertices[0], GL_DYNAMIC_DRAW);

		m_shader->bind();

		m_shader->setUniform("world", glm::mat4(1.0f));
		m_shader->setUniform("view", GameObject::s_camera->view());
		m_shader->setUniform("projection", GameObject::s_camera->projection());
	
		glDrawArrays(GL_LINES, 0, m_vertices.size());

		m_shader->unbind();

		glBindVertexArray(0);
	}
}