#include "GameObject.h"
#include "ParticleSystem.h"
#include "ParticleRenderer.h"
#include "Utils.h"

#include <GL/glew.h>

namespace Common
{
	ParticleRenderer::ParticleRenderer(Shader * shader, Texture * texture, std::vector<Particle> & particles)
		: Renderer(0), m_shader(shader), m_texture(texture), m_particles(particles)
	{
		glGenVertexArrays(1, &m_vaoID);
		glBindVertexArray(m_vaoID);

		glGenBuffers(1, &m_vboID);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

		glVertexAttribPointer(Shader::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
		glEnableVertexAttribArray(Shader::POSITION);

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
		glBindVertexArray(m_vaoID);

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		glBufferData(GL_ARRAY_BUFFER, m_particles.size() * sizeof(Particle), &m_particles[0], GL_DYNAMIC_DRAW);

		m_texture->bind();
		m_shader->bind();

		m_shader->setUniform("world", glm::mat4(1.0f));
		m_shader->setUniform("view", GameObject::s_camera->view());
		m_shader->setUniform("projection", GameObject::s_camera->projection());

		glDrawArrays(GL_POINTS, 0, m_particles.size());

		m_shader->unbind();
		m_texture->unbind();

		glBindVertexArray(0);
	}
}