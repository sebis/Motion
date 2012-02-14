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

#include "BVH.h"
#include "CollisionDetector.h"
#include "GameObject.h"
#include "Trace.h"

#include <sstream>

namespace
{
	// From Real-Time Collision Detection by Christopher Ericson. Uses Lagrange's identity.
	bool PointInTriangle(const glm::vec3 & p, const Common::Triangle * tri, Common::Contact * contact, float threshold)
	{
		const glm::vec3 & a = tri->a - p;
		const glm::vec3 & b = tri->b - p;
		const glm::vec3 & c = tri->c - p;

		float ab = glm::dot(a, b);
		float ac = glm::dot(a, c);
		float bc = glm::dot(b, c);
		float cc = glm::dot(c, c);

		if (bc * ac - cc * ab < 0.0f)
			return false;

		float bb = glm::dot(b, b);
		if (ab * bc - ac * bb < 0.0f)
			return false;

		// distance of point p along normal
		contact->penetration = glm::dot(tri->n, p - tri->a);
		if (contact->penetration > threshold)
			return false;

		// collision normal is same as triangle normal
		contact->normal = tri->n;

		// projected point on triangle
		contact->point = p + contact->normal * (threshold - contact->penetration);

		return true;
	}

	bool PointInBVH(const Common::BVHNode * node, const glm::vec3 & p, Common::Contact * contact, float threshold)
	{
		if (!node->isInside(p))
			return false;

		if (node->m_isLeaf) {
			return PointInTriangle(p, node->m_triangle, contact, threshold);
		} else {
			if (PointInBVH(node->left, p, contact, threshold))
				return true;
			if (PointInBVH(node->right, p, contact, threshold))
				return true;
		}

		return false;
	}
}

namespace Common
{
	bool SphereCollider::isInside(const glm::vec3 & point, Contact * contact) const
	{
		const glm::vec3 & diff = point - m_position;
		float dist = glm::length(diff);

		float t = CollisionDetector::COLLISION_THRESHOLD;

		if (dist < (m_radius + t)) {

			contact->normal = glm::normalize(diff);
			contact->penetration = dist - (m_radius + t);
			contact->point = contact->normal * (m_radius + t);

			return true;
		}

		return false;
	}

	bool MeshCollider::isInside(const glm::vec3 & point, Contact * contact) const
	{
		return PointInBVH(m_bvh->root(), point, contact, CollisionDetector::COLLISION_THRESHOLD);
	}

	//////////////////////////////////////
	//////////////////////////////////////

	float CollisionDetector::COLLISION_THRESHOLD = 0.0f;

	CollisionDetector * CollisionDetector::s_instance = 0;

	CollisionDetector * CollisionDetector::instance()
	{
		if (s_instance == 0)
			s_instance = new CollisionDetector();

		return s_instance;
	}

	bool CollisionDetector::SphereAndSphere(SphereCollider * a, SphereCollider * b, CollisionData * data)
	{
		glm::vec3 pa = glm::vec3(a->transform().world() * glm::vec4(a->m_position, 1.0f));
		glm::vec3 pb = glm::vec3(b->transform().world() * glm::vec4(b->m_position, 1.0f));

		glm::vec3 ab = pa - pb;
		float length = glm::length(ab);

		if (length <= 0.0f || length > a->m_radius + b->m_radius)
			return false;

		Contact * contact = new Contact;

		contact->normal = glm::normalize(ab);
		contact->point = pa + 0.5f * ab;
		contact->penetration = a->m_radius + b->m_radius - length;

		data->contacts.push_back(contact);

		return true;
	}
	
	bool CollisionDetector::SphereAndPlane(SphereCollider * sphere, PlaneCollider * plane, CollisionData * data)
	{
		glm::vec3 position = glm::vec3(sphere->transform().world() * glm::vec4(sphere->m_position, 1.0f));

		float distance = glm::dot(plane->m_normal, position) - sphere->m_radius - plane->m_d;

		if (distance >= 0)
			return false;

		Contact * contact = new Contact;

		contact->normal = plane->m_normal;
		contact->point = position - plane->m_normal * (distance + sphere->m_radius);
		contact->penetration = -distance;

		data->contacts.push_back(contact);

		return true;
	}

	// TODO: create a proper point collider and it to the collision detection algorithm
	Contact * CollisionDetector::collides(const glm::vec3 & position)
	{
		Contact * contact = new Contact;

		for (unsigned i = 0; i < m_colliders.size(); i++)
		{
			Collider * collider = m_colliders[i];

			// transform position to local coordinates
			const glm::mat4 & invWorld = collider->transform().invWorldMatrix();
			const glm::vec3 & p = glm::vec3(invWorld * glm::vec4(position, 1.0f));

			if (collider->isInside(p, contact)) {

				// transform back to world coordinates
				const glm::mat4 & world = collider->transform().worldMatrix();

				contact->normal = glm::vec3(world * glm::vec4(contact->normal, 0.0f));
				contact->point = glm::vec3(world * glm::vec4(contact->point, 1.0f));

				return contact;
			}
		}

		delete contact;

		return 0;
	}

	void CollisionDetector::detectCollisions(std::vector<CollisionData> & collisions)
	{
		collisions.clear();

		// TODO: do this completely over
		for (unsigned i = 0; i < m_colliders.size(); i++)
		{
			for (unsigned j = i+1; j < m_colliders.size(); j++)
			{
				if (j >= m_colliders.size())
					break;

				CollisionData data;

				MeshCollider * mesh1 = dynamic_cast<MeshCollider*>(m_colliders[i]);
				MeshCollider * mesh2 = dynamic_cast<MeshCollider*>(m_colliders[j]);
				if (mesh1 || mesh2)
					continue; // mesh collision not implemented

				PlaneCollider * plane = dynamic_cast<PlaneCollider*>(m_colliders[i]);
				if (plane)
				{
					SphereCollider * sphere = dynamic_cast<SphereCollider*>(m_colliders[j]);
					if (sphere) {
						CollisionData * pData = &data;
						if (CollisionDetector::SphereAndPlane(sphere, plane, pData)) {
							data.bodies[0] = sphere->collisionBody();
							data.bodies[1] = plane->collisionBody();
							data.restitution = COF_BALL_TABLE;
							data.friction = FRICTION_BALL_CLOTH;
							collisions.push_back(data);
						}
						continue;
					}

					continue; // TODO: plane-plane not implemented
				}

				SphereCollider * sphere = dynamic_cast<SphereCollider*>(m_colliders[i]);
				if (sphere)
				{
					PlaneCollider * plane = dynamic_cast<PlaneCollider*>(m_colliders[j]);
					if (plane) {
						if (SphereAndPlane(sphere, plane, &data)) {
							data.bodies[0] = sphere->collisionBody();
							data.bodies[1] = plane->collisionBody();
							data.restitution = COF_BALL_TABLE;
							data.friction = FRICTION_BALL_CLOTH;
							collisions.push_back(data);
						}
						continue;
					}

					SphereCollider * sphere2 = dynamic_cast<SphereCollider*>(m_colliders[j]);
					if (sphere2) {
						if (SphereAndSphere(sphere, sphere2, &data)) {
							data.bodies[0] = sphere->collisionBody();
							data.bodies[1] = sphere2->collisionBody();
							data.restitution = COF_BALL_BALL;
							data.friction = FRICTION_BALL_BALL;
							collisions.push_back(data);
						}
						continue;
					}

					assert(false);
				}

				assert(false);
			}
		}
	}
}