#ifndef COMMON_PARTICLESYSTEM
#define COMMON_PARTICLESYSTEM

#include "ParticleRenderer.h"

#include "glm/glm.hpp"

#include <vector>

namespace Common
{
	struct Particle
	{
		glm::vec3 position;
		glm::vec3 velocity;
		
		float startAlpha;
		glm::vec4 color;

		float startSize;
		float endSize;
		float size;

		float duration;
		glm::float_t age;
	};

	struct ParticleSettings
	{
		unsigned maxParticles;

		glm::vec3 gravity;

		float minDuration;
		float maxDuration;

		glm::vec3 minVelocity;
		glm::vec3 maxVelocity;

		float minStartSize;
		float maxStartSize;

		float minEndSize;
		float maxEndSize;

		glm::vec4 minColor;
		glm::vec4 maxColor;

		float minStartAlpha;
		float maxStartAlpha;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem();
		virtual ~ParticleSystem();

		void update(float dt);
		void draw();

		void addParticle(const glm::vec3 & position, const glm::vec3 & velocity);

	private:
		typedef std::vector<Particle> Particles;
		typedef Particles::iterator ParticleIterator;
		Particles m_particles;

		ParticleRenderer * m_renderer;
		ParticleSettings * m_settings;
	};
}

#endif /* COMMON_PARTICLESYSTEM */