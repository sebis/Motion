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

#ifndef COMMON_COLLISIONDETECTOR
#define COMMON_COLLISIONDETECTOR

#include "Collider.h"
#include "CollisionBody.h"

#include <vector>
#include <string>

namespace Common
{
	namespace
	{
		const float COF_BALL_BALL = 0.95f;
		const float COF_BALL_RAIL = 0.7f;
		const float COF_BALL_TABLE = COF_BALL_RAIL;
	
		const float FRICTION_BALL_BALL = 0.05f;
		const float FRICTION_BALL_CLOTH = 0.5f;
	}

	struct Contact
	{
		glm::vec3 point;
		glm::vec3 normal;
		float penetration;

		void * userData;
	};

	struct CollisionData
	{
		enum Type {
			RIGID,
			SOFT
		};

		std::vector<Contact*> contacts;
		float restitution;
		float friction;

		Type type;
		CollisionBody * bodies[2];
	};

	class CollisionDetector
	{
	public:
		static CollisionDetector * instance();
		~CollisionDetector() {}

		static bool SphereAndSphere(SphereCollider * a, SphereCollider * b, CollisionData * data);
		static bool SphereAndPlane(SphereCollider * a, PlaneCollider * b, CollisionData * data);

		inline void addCollider(Collider * collider) { m_colliders.push_back(collider); }
		inline void clear() { m_colliders.clear(); }

		Contact * collides(const glm::vec3 & position);
		void detectCollisions(std::vector<CollisionData> & collisions);

		static float COLLISION_THRESHOLD;

	private:
		CollisionDetector() {}

		typedef std::vector<Collider*> Colliders;
		Colliders m_colliders;

		static CollisionDetector * s_instance;
	};
}

#endif /* COMMON_COLLISIONDETECTOR */