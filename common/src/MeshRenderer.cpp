#include "GameObject.h"
#include "MeshRenderer.h"

namespace Common
{
	MeshRenderer::MeshRenderer(GameObject * gameObject, Mesh * mesh, Shader * shader, Texture * texture)
		: Renderer(gameObject), m_mesh(mesh), m_shader(shader), m_texture(texture)
	{
	}

	MeshRenderer::~MeshRenderer()
	{
		delete m_texture;
	}

	void MeshRenderer::draw()
	{
		// TODO: proper way to change render states
		glDisable(GL_BLEND);

		if (m_texture)
			m_texture->bind();

		m_shader->bind();
		m_shader->setUniform("world", m_gameObject->m_transform.world());
		m_shader->setUniform("view", m_gameObject->m_camera->view());
		m_shader->setUniform("projection", m_gameObject->m_camera->projection());

		m_mesh->draw();

		m_shader->unbind();

		if (m_texture)
			m_texture->unbind();

		glEnable(GL_BLEND);
	}
};