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

#ifndef COMMON_TRANSFORM
#define COMMON_TRANSFORM

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Common
{
	class Transform
	{
	public:
		enum RotationType
		{
			EULER		= (1 << 0),
			MATRIX		= (1 << 1),
			QUATERNION	= (1 << 2)
		};

		Transform(RotationType rotationType = EULER)
			: m_rotationType(rotationType),
			m_position(glm::vec3(0.0f)),
			m_rotation(glm::vec3(0.0f)),
			m_scale(glm::vec3(1.0f)),
			m_parent(0)
		{}

		static Transform Translation(const float& x, const float& y, const float& z)
		{
			return Translation(glm::vec3(x, y, z));
		}

		static Transform Translation(const glm::vec3& translate)
		{
			Transform t;
			t.translate(translate);
			return t;
		}

		inline void translate(const glm::vec3& translate)
		{
			m_position += translate;
		}

		inline void rotate(const glm::vec3& rotate)
		{
			m_rotation += rotate;
		}

		inline void scale(const glm::vec3& scale)
		{
			m_scale *= scale;
		}

		const glm::mat4 world() const;
		static Transform extract(const glm::mat4& m, RotationType rotationType = EULER);

		inline const Transform * parent() const { return m_parent; }
		inline void setParent(Transform * parent) { m_parent = parent; }
		inline void setParent(Transform& parent) { m_parent = &parent; }

		inline glm::vec3& position() { return m_position; }
		inline const glm::vec3 position() const { return m_position; }

		inline glm::vec3& rotation() { return m_rotation; }
		inline const glm::vec3& rotation() const { return m_rotation; }

		inline glm::vec3& scale() { return m_scale; }
		inline const glm::vec3& scale() const { return m_scale; }

		inline glm::quat& quaternion() { return m_quaternion; }

		inline glm::mat3& rotationMatrix() { return m_rotationMatrix; }
		inline const glm::mat3& rotationMatrix() const { return m_rotationMatrix; }

		inline void enableQuaternions() { m_rotationType = QUATERNION; }

		inline Transform operator*(const Transform& other) const
		{
			return Transform::extract(world() * other.world());
		}

		inline void update()
		{
			m_worldMatrix = world();
			m_invWorld = glm::inverse(m_worldMatrix);
		}

		inline const glm::mat4 & worldMatrix() const { return m_worldMatrix; }
		inline const glm::mat4 & invWorldMatrix() const { return m_invWorld; }

	private:
		RotationType m_rotationType;

		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;

		glm::mat3 m_rotationMatrix;
		glm::quat m_quaternion;
	
		Transform * m_parent;

		glm::mat4 m_worldMatrix;
		glm::mat4 m_invWorld;
	};
}

#endif /* COMMON_TRANSFORM */
