#ifndef COMMON_PARTICLEEMITTER
#define COMMON_PARTICLEEMITTER

#include "Component.h"
#include "ParticleSystem.h"

#include "glm/glm.hpp"

namespace Common
{
	class ParticleEmitter : public Component
	{
	public:
		ParticleEmitter(ParticleSystem * system, float particlesPerSecond, const glm::vec3 & position = glm::vec3(0.0f), const glm::vec3 & variance = glm::vec3(0.0f))
			: m_system(system), m_particlesPerSecond(particlesPerSecond), m_position(position), m_variance(variance) {}
		virtual ~ParticleEmitter() {}

		virtual void update(float dt);

		inline int available(float dt)
		{
			return int(m_particlesPerSecond * (dt / 1000.0f));
		}

	protected:
		ParticleSystem * m_system;
		float m_particlesPerSecond;
		glm::vec3 m_position;
		glm::vec3 m_variance;
	};

	class CircleParticleEmitter : public ParticleEmitter
	{
	public:
		CircleParticleEmitter(ParticleSystem * system, float particlesPerSecond, float radius)
			: ParticleEmitter(system, particlesPerSecond), m_radius(radius) {}
		virtual ~CircleParticleEmitter() {}
		
		virtual void update(float dt);
		float radius() const { return m_radius; }

	private:
		float m_radius;
	};

	class SphereParticleEmitter : public CircleParticleEmitter
	{
	public:
		SphereParticleEmitter(ParticleSystem * system, float particlesPerSecond, float radius)
			: CircleParticleEmitter(system, particlesPerSecond, radius) {}
		virtual ~SphereParticleEmitter() {}

		void update(float dt);
	};

	class TrailParticleEmitter : public ParticleEmitter
	{
	public:
		TrailParticleEmitter(ParticleSystem * system, float particlesPerSecond)
			: ParticleEmitter(system, particlesPerSecond) {}
		virtual ~TrailParticleEmitter() {}

		void update(float dt);
		inline void addPosition(const glm::vec3 & position) { m_positions.push_back(position); }
		
	private:
		std::vector<glm::vec3> m_positions;
	};
}

#endif /* COMMON_PARTICLEEMITTER */