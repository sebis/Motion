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

#ifndef COMMON_PARTICLESYSTEM
#define COMMON_PARTICLESYSTEM

#include "Component.h"
#include "FlowPrimitive.h"
#include "ParticleRenderer.h"

#include "glm/glm.hpp"

#include <vector>

namespace Common
{
	struct Particle
	{
		glm::vec3 position;
		glm::vec3 velocity;
		
		glm::vec4 color;

		float startSize;
		float endSize;
		float size;

		float age;

		float ageRandom;
		float colorRandom;
	};

	struct ParticleSettings
	{
		unsigned maxParticles;

		std::string texture;

		float duration;

		glm::vec3 minVelocity;
		glm::vec3 maxVelocity;

		float minStartSize;
		float maxStartSize;

		float minEndSize;
		float maxEndSize;

		glm::vec4 minColor;
		glm::vec4 maxColor;

		GLenum srcBlend;
		GLenum dstBlend;
	};

	class ParticleSystem : public Component
	{
	public:
		ParticleSystem(const ParticleSettings & settings);
		virtual ~ParticleSystem();

		void update(float dt);
		void draw();

		void addParticle(const glm::vec3 & position, const glm::vec3 & velocity);
		void addFlow(FlowPrimitive * flow);

		inline void setShader(Shader * shader) { m_renderer->setShader(shader); }

	private:
		const ParticleSettings m_settings;

		unsigned firstActiveParticle;
		unsigned firstFreeParticle;

		Particle * m_particles;
		ParticleRenderer * m_renderer;

		typedef std::vector<FlowPrimitive*> Flows;
		typedef Flows::iterator FlowIterator;
		Flows m_flows;
	};
}

#endif /* COMMON_PARTICLESYSTEM */
