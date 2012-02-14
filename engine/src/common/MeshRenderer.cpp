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
#include "MeshRenderer.h"

namespace Common
{
	MeshRenderer::MeshRenderer(GameObject * gameObject, Mesh * mesh, Material * material)
		: Renderer(gameObject), m_mesh(mesh), m_material(material)
	{
	}

	MeshRenderer::~MeshRenderer()
	{
	}

	void MeshRenderer::draw()
	{
		// TODO: proper way to change render states
		glDisable(GL_BLEND);

		m_material->begin();

		// TODO: better way to get transform from rigidbody
		if (m_gameObject->m_rigidbody) {
			m_material->shader()->setUniform("world", m_gameObject->m_rigidbody->transform().world());
		} else {
			m_material->shader()->setUniform("world", m_gameObject->transform().world());
		}

		m_material->shader()->setUniform("view", GameObject::s_camera->view());
		m_material->shader()->setUniform("projection", GameObject::s_camera->projection());

		m_mesh->draw();

		m_material->end();

		glEnable(GL_BLEND);
	}
};
