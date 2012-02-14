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

#ifndef COMMON_BONE
#define COMMON_BONE

#include "MeshObject.h"
#include "Transform.h"

#include <string>
#include <vector>

namespace Common
{
	class RotationAxis
	{
	public:
		inline void setAxis(const glm::vec3& axis)
		{
			m_axis = axis;
		}

		inline void setConstraints(float min, float max)
		{
			m_min = min;
			m_max = max;
		}

		inline bool valid(float value)
		{
			return m_min <= value && value <= m_max;
		}

		inline bool free()
		{
			return glm::abs(m_max - m_min) > 0;
		}

		glm::vec3 m_axis;
		float m_min;
		float m_max;
		
	};

	class Bone
	{
	public:

		Bone(const std::string& name, Transform& transform, Bone * parent = 0);
		virtual ~Bone() {}

		inline Transform& transform() { return m_transform; }
		inline const Transform& transform() const { return m_transform; }

		inline Transform& localTransform() { return m_localTransform; }
		inline Bone * parent() const { return m_parent; }
		inline const std::string& name() const { return m_name; }

		inline RotationAxis * axes() { return m_axes; }
		inline RotationAxis * axis(int i) { return &m_axes[i]; }
		
	private:
		std::string m_name;

		Transform& m_transform;
		RotationAxis m_axes[3];

		Transform m_localTransform;

		Bone * m_parent;
	};
}

#endif /* COMMON_BONE */
