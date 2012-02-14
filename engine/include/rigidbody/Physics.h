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

#ifndef COMMON_PHYSICS
#define COMMON_PHYSICS

#include "LineRenderer.h"
#include "RigidBody.h"

#include <vector>

namespace Common
{
	struct CollisionData;

	class Physics
	{
	public:
		Physics();
		~Physics();

		void addObject(RigidBody * rigidBody);
		void removeObject(RigidBody * rigidBody);

		void step(float dt);
		void visualize();

		void explode(const glm::vec3 & center);

		float calculateEnergy(float potentialZero = 0.0f);

	private:
		void resolveVelocity(const CollisionData & data);
		void resolveInterpenetration(const CollisionData & data);

		typedef std::vector<RigidBody *> Objects;
		typedef Objects::iterator ObjectIterator;

		Objects m_objects;

		LineRenderer * m_lineRenderer;
	};
}

#endif /* COMMON_PHYSICS */
