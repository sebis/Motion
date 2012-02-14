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

#include "ParticleEmitter.h"
#include "Utils.h"

namespace Common
{
	namespace
	{
		Utils::Random r;
	}

	void ParticleEmitter::update(float dt)
	{
		for (int i = 0; i < available(dt); i++)
		{
			m_system->addParticle(m_position + float(r.rand11()) * m_variance, glm::vec3(0.0f));
		}
	}

	void CircleParticleEmitter::update(float dt)
	{
		for (int i = 0; i < available(dt); i++)
		{
			float angle = float(r.rand01() * 2 * M_PI);
			glm::vec3 position(m_radius * glm::cos(angle), 0.0f, m_radius * glm::sin(angle));

			m_system->addParticle(position, glm::vec3(0.0f));
		}
	}

	void SphereParticleEmitter::update(float dt)
	{
		for (int i = 0; i < available(dt); i++)
		{
			float omega = float(r.rand01() * M_PI);
			float phi = float(r.rand01() * 2 * M_PI);

			float r = radius();

			float x = r * glm::sin(omega) * glm::cos(phi);
			float y = r * glm::sin(omega) * glm::sin(phi);
			float z = r * glm::cos(omega);

			m_system->addParticle(glm::vec3(x, y, z), glm::vec3(0.0f));
		}
	}

	void TrailParticleEmitter::update(float dt)
	{
		if (m_positions.empty())
			return;

		for (int i = 0; i < available(dt); i++)
		{
			unsigned index = unsigned(r.rand01() * m_positions.size());
			if (index == m_positions.size())
				continue;

			m_system->addParticle(m_positions[index], glm::vec3(0.0f));
		}
	}
}
