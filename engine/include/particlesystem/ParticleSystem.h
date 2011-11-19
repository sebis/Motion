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
		glm::float_t age;
	};

	struct ParticleSettings
	{
		glm::vec3 gravity;
		float duration;

		glm::vec3 minVelocity;
		glm::vec3 maxVelocity;
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