#include "Material.h"

namespace Common
{
	Material::Material(Shader * shader)
		: m_shader(shader),
		  m_ambientColor(glm::vec4(0.0f)),
		  m_diffuseColor(glm::vec4(0.5f)),
		  m_specularColor(glm::vec4(1.0f)),
		  m_shininess(30.0f),
		  m_texture(0)
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

		if (m_texture)
			m_texture->unbind();
	}
}