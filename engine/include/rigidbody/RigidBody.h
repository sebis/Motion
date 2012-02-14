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

#ifndef COMMON_RIGIDBODY
#define COMMON_RIGIDBODY

#include "CollisionBody.h"
#include "Transform.h"

#include "glm/glm.hpp"

#include <vector>

namespace Common
{
	class GameObject;

	class RigidBody : public CollisionBody
	{
	public:
		RigidBody(GameObject * gameObject);
		virtual ~RigidBody();

		inline void applyForce(const glm::vec3 & force)
		{
			m_forceAccumulator += force;
		}

		inline void applyTorque(const glm::vec3 & torque)
		{
			m_torqueAccumulator += torque;
		}

		void applyImpulse(const glm::vec3 & impulse, const glm::vec3 & point);

		inline Transform transform() const
		{
			Transform t(Transform::MATRIX);

			t.position() = m_position;
			t.scale() = m_scale;
			t.rotationMatrix() = m_rotation;

			return t;
		}

		inline const glm::vec3 & position() const { return m_position; }
		inline const glm::vec3 & velocity() const { return m_velocity; }

	private:
		// TODO: do access some other way?
		friend class Physics;

		void integrate(float dt);

		GameObject * m_gameObject;

		glm::vec3 m_position;
		glm::mat3 m_rotation;
		glm::vec3 m_scale;

		glm::float_t m_damping;
		glm::float_t m_angularDamping;
		glm::float_t m_mass;
		glm::mat3 m_inertiaTensor;

		glm::vec3 m_velocity;
		glm::vec3 m_angularVelocity;

		glm::vec3 m_acceleration;
		glm::vec3 m_angularAcceleration;

		glm::vec3 m_forceAccumulator;
		glm::vec3 m_torqueAccumulator;
	};
}
#endif /* COMMON_RIGIDBODY */
