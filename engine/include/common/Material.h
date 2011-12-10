#ifndef COMMON_MATERIAL
#define COMMON_MATERIAL

#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"

namespace Common
{
	class Material
	{
	public:
		Material(Shader * shader);
		virtual ~Material();

		void begin();
		void end();
		
		void setTexture(Texture * texture);
		inline Shader * shader() const { return m_shader; }

		inline const glm::vec4& ambientColor() const { return m_ambientColor; }
		inline const glm::vec4& diffuseColor() const { return m_diffuseColor; }
		inline const glm::vec4& specularColor() const { return m_specularColor; }
		inline const glm::float_t& shininess() const { return m_shininess; }

		inline void setAmbientColor(const glm::vec4& ambientColor) { m_ambientColor = ambientColor; }
		inline void setDiffuseColor(const glm::vec4& diffuseColor) { m_diffuseColor = diffuseColor; }
		inline void setSpecularColor(const glm::vec4& specularColor) { m_specularColor = specularColor; }
		inline void setShininess(const glm::float_t& shininess) { m_shininess = shininess; }

		inline void setWireframe(bool wireframe) { m_wireframe = wireframe; }

	private:
		Shader * m_shader;
		Texture * m_texture;

		glm::vec4 m_ambientColor;
		glm::vec4 m_diffuseColor;
		glm::vec4 m_specularColor;
		glm::float_t m_shininess;

		bool m_wireframe;
	};
}

#endif /* COMMON_MATERIAL */
