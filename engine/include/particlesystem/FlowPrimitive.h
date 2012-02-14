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

#ifndef COMMON_FLOWPRIMITIVE
#define COMMON_FLOWPRIMITIVE

#include "Trace.h"

#include "glm/glm.hpp"

namespace Common
{
	class FlowPrimitive
	{
	public:
		FlowPrimitive() {}
		virtual ~FlowPrimitive() {}
		
		virtual glm::vec3 velocityAt(const glm::vec3 & position) = 0;
	};

	class UniformFlow : public FlowPrimitive
	{
	public:
		UniformFlow(const glm::vec3 & velocity, const glm::vec3 & center = glm::vec3(0.0f), float radius = +1e10)
			: m_center(center), m_velocity(velocity), m_radius(radius) {}
		virtual ~UniformFlow() {}

		glm::vec3 velocityAt(const glm::vec3 & position);

	private:
		glm::vec3 m_center;
		glm::vec3 m_velocity;
		float m_radius;
	};

	class SourceFlow : public FlowPrimitive
	{
	public:
		SourceFlow(const glm::vec3 & center, float radius, float magnitude)
			: m_center(center), m_radius(radius), m_magnitude(magnitude) {}
		virtual ~SourceFlow() {}

		glm::vec3 velocityAt(const glm::vec3 & position);

	private:
		glm::vec3 m_center;
		float m_radius;
		float m_magnitude;
	};

	class VortexFlow : public FlowPrimitive
	{
	public:
		VortexFlow(const glm::vec3 & center, const glm::vec3 & axis, float radius, float magnitude)
			: m_center(center), m_axis(axis), m_radius(radius), m_magnitude(magnitude) {}
		virtual ~VortexFlow() {}

		glm::vec3 velocityAt(const glm::vec3 & position);

	private:
		glm::vec3 m_center;
		glm::vec3 m_axis;
		float m_radius;
		float m_magnitude;
	};
}

#endif /* COMMON_FLOWPRIMITIVE */