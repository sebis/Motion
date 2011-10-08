#include "GameObject.h"
#include "MeshRenderer.h"

namespace Common
{
	MeshRenderer::MeshRenderer(GameObject * gameObject, Mesh * mesh, Shader * shader)
		: Renderer(gameObject), m_mesh(mesh), m_shader(shader)
	{
	}

	void MeshRenderer::draw()
	{
		m_shader->bind();
		m_shader->setUniform("world", m_gameObject->m_transform.world());
		m_shader->setUniform("view", m_gameObject->m_camera->view());
		m_shader->setUniform("projection", m_gameObject->m_camera->projection());

		m_mesh->draw();

		m_shader->unbind();
	}
};