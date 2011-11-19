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
		ParticleRenderer(Shader * shader, Texture * texture, std::vector<Particle> & particles);
		virtual ~ParticleRenderer();

		void draw();

		inline void setParticles(std::vector<Particle> & particles) { m_particles = particles; }

	private:
		Shader * m_shader;
		Texture * m_texture;

		glm::uint m_vaoID;
		glm::uint m_vboID;

		std::vector<Particle> & m_particles;
	};
}

#endif /* COMMON_PARTICLERENDERER */