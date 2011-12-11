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
