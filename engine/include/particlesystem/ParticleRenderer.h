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

#ifndef COMMON_PARTICLERENDERER
#define COMMON_PARTICLERENDERER

#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"

#include <vector>

namespace Common
{
	struct Particle;

	class ParticleRenderer : public Renderer
	{
	public:
		ParticleRenderer(Texture * texture);
		virtual ~ParticleRenderer();

		void draw();
		void draw(Particle * first, unsigned size);

		inline void setShader(Shader * shader) { m_shader = shader; }
		inline void setBlendMode(GLenum src, GLenum dst) { m_srcBlend = src; m_dstBlend = dst; }

		static bool s_renderPoints;

	private:
		Shader * m_shader;
		Texture * m_texture;

		GLenum m_srcBlend;
		GLenum m_dstBlend;

		glm::uint m_vaoID;
		glm::uint m_vboID;
	};
}

#endif /* COMMON_PARTICLERENDERER */