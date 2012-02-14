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
