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