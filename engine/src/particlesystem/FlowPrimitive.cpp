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

#include "FlowPrimitive.h"

namespace Common
{
	glm::vec3 UniformFlow::velocityAt(const glm::vec3 & position)
	{
		glm::vec3 r = position - m_center;
		float d = glm::length(r);

		if (d <= m_radius)
			return m_velocity;

		return glm::vec3(0.0f);
	}

	glm::vec3 SourceFlow::velocityAt(const glm::vec3 & position)
	{
		glm::vec3 r = position - m_center;
		float d = glm::length(r);
		float r0 = m_radius;
		float m = m_magnitude;

		if (d <= r0)
			return (1 - d / r0) * glm::normalize(r) * m;

		return glm::vec3(0.0f);
	}

	glm::vec3 VortexFlow::velocityAt(const glm::vec3 & position)
	{
		glm::vec3 r = position - m_center;
		float d = glm::length(r);
		float r0 = m_radius;
		float m = m_magnitude;

		if (d <= r0)
			return (1 - d / r0) * glm::normalize(glm::cross(r, m_axis)) * m;

		return glm::vec3(0.0f);
	}
}
