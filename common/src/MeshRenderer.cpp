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

		m_material->shader()->setUniform("world", m_gameObject->transformMatrix());
		m_material->shader()->setUniform("view", m_gameObject->m_camera->view());
		m_material->shader()->setUniform("projection", m_gameObject->m_camera->projection());

		m_mesh->draw();

		m_material->end();

		glEnable(GL_BLEND);
	}
};