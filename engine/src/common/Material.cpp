#include "Material.h"

namespace Common
{
	Material::Material(Shader * shader)
		: m_shader(shader),
		  m_texture(0),
		  m_ambientColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)),
		  m_diffuseColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)),
		  m_specularColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
		  m_shininess(30.0f),
		  m_wireframe(false)
	{
	}
	
	Material::~Material()
	{
		delete m_texture;
	}

	void Material::setTexture(Texture * texture)
	{
		if (texture == 0)
			delete m_texture;

		m_texture = texture;
	}

	void Material::begin()
	{
		if (m_texture)
			m_texture->bind();

		if (m_wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		m_shader->bind();

		// TODO: light should come from a global stuff?
		m_shader->setUniform("light_Position", glm::vec4(10.0f, 10.0f, 10.0f, 0.0f));
		// TODO: handle binding several textures
		m_shader->setUniform("useTexture", m_texture != 0);

		m_shader->setUniform("ambientColor", m_ambientColor);
		m_shader->setUniform("diffuseColor", m_diffuseColor);
		m_shader->setUniform("specularColor", m_specularColor);
		m_shader->setUniform("shininess", m_shininess);
	}

	void Material::end()
	{
		m_shader->unbind();

		if (m_wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (m_texture)
			m_texture->unbind();
	}
}
