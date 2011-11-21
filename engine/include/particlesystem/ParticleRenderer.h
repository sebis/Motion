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