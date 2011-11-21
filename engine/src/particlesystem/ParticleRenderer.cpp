#include "GameObject.h"
#include "ParticleSystem.h"
#include "ParticleRenderer.h"
#include "Utils.h"

#include <GL/glew.h>

namespace Common
{
	bool ParticleRenderer::s_renderPoints = false;

	ParticleRenderer::ParticleRenderer(Texture * texture)
		: Renderer(0), m_texture(texture)
	{
		m_shader = Shader::find(s_renderPoints ? "point" : "particle");

		glGenVertexArrays(1, &m_vaoID);
		glBindVertexArray(m_vaoID);

		glGenBuffers(1, &m_vboID);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

		glVertexAttribPointer(Shader::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
		glEnableVertexAttribArray(Shader::POSITION);
		glVertexAttribPointer(Shader::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));
		glEnableVertexAttribArray(Shader::COLOR);
		glVertexAttribPointer(Shader::POINTSIZE, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, size));
		glEnableVertexAttribArray(Shader::POINTSIZE);

		glBindVertexArray(0);
	}

	ParticleRenderer::~ParticleRenderer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_vboID);

		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vaoID);
	}

	void ParticleRenderer::draw()
	{
	}

	void ParticleRenderer::draw(Particle * first, unsigned size)
	{
		glBindVertexArray(m_vaoID);

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(Particle), first, GL_DYNAMIC_DRAW);

		m_texture->bind();
		m_shader->bind();

		m_shader->setUniform("world", glm::mat4(1.0f));
		m_shader->setUniform("view", GameObject::s_camera->view());
		m_shader->setUniform("projection", GameObject::s_camera->projection());
		m_shader->setUniform("aspect", Camera::s_aspect);

		// TODO: these should be set to original values instead of disabling
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(m_srcBlend, m_dstBlend);

		glDrawArrays(GL_POINTS, 0, size);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		m_shader->unbind();
		m_texture->unbind();

		glBindVertexArray(0);
	}
}