#ifndef COMMON_PARTICLEEMITTER
#define COMMON_PARTICLEEMITTER

#include "Component.h"
#include "ParticleSystem.h"

namespace Common
{
	class ParticleEmitter : public Component
	{
	public:
		ParticleEmitter(ParticleSystem * system, float particlesPerSecond)
			: m_system(system), m_particlesPerSecond(particlesPerSecond) {}
		virtual ~ParticleEmitter() {}

		virtual void update(float dt) = 0;

		inline int available(float dt)
		{
			return int(m_particlesPerSecond * (dt / 1000.0f));
		}

	protected:
		ParticleSystem * m_system;
		float m_particlesPerSecond;
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