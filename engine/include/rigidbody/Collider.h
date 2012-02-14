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

#ifndef COMMON_COLLIDER
#define COMMON_COLLIDER

#include "BVH.h"
#include "CollisionBody.h"
#include "Mesh.h"
#include "Transform.h"

#include "glm/glm.hpp"

#include <string>

namespace Common
{
	class GameObject;
	struct Contact;

	class Collider
	{
	public:
		Collider(GameObject * gameObject, CollisionBody * body = 0) : m_gameObject(gameObject), m_collisionBody(body) {}
		virtual ~Collider() {}

		std::string name;

		inline CollisionBody * collisionBody() const { return m_collisionBody; }
		virtual bool isInside(const glm::vec3 & /*point*/, Contact * /*contact*/) const { return false; }

		Transform transform() const;

	private:
		GameObject * m_gameObject;
		CollisionBody * m_collisionBody;
	};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider(GameObject * gameObject, CollisionBody * body = 0) 
			: Collider(gameObject, body),
			m_position(glm::vec3(0.0f)),
			m_radius(1.0f) {}

		virtual ~SphereCollider() {}

		virtual bool isInside(const glm::vec3 & point, Contact * contact) const;

		glm::vec3 m_position;
		glm::float_t m_radius;
	};

	class PlaneCollider : public Collider
	{
	public:
		PlaneCollider(GameObject * gameObject, CollisionBody * body = 0) : Collider(gameObject, body) {}
		virtual ~PlaneCollider() {}

		glm::vec3 m_normal;
		glm::float_t m_d;
	};

	class MeshCollider : public Collider
	{
	public:
		MeshCollider(GameObject * gameObject, CollisionBody * body = 0, bool usePoints = false) : Collider(gameObject, body), m_usePoints(usePoints) {}
		virtual ~MeshCollider() {}

		virtual bool isInside(const glm::vec3 & point, Contact * contact) const;

		Mesh * m_mesh;
		BVH * m_bvh;
		bool m_usePoints;
	};
}

#endif /* COMMON_COLLIDER */
