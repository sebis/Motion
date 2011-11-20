#include "CollisionDetector.h"
#include "Trace.h"

#include <sstream>

namespace
{
	
}

namespace Common
{
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

		data->normal = glm::normalize(ab);
		data->point = pa + 0.5f * ab;
		data->penetration = a->m_radius + b->m_radius - length;

		std::stringstream ss;
		ss << a->name << " vs " << b->name;
		data->result = ss.str();

		return true;
	}
	
	bool CollisionDetector::SphereAndPlane(SphereCollider * sphere, PlaneCollider * plane, CollisionData * data)
	{
		glm::vec3 position = glm::vec3(sphere->transform().world() * glm::vec4(sphere->m_position, 1.0f));

		float distance = glm::dot(plane->m_normal, position) - sphere->m_radius - plane->m_d;

		if (distance >= 0)
			return false;

		data->normal = plane->m_normal;
		data->point = position - plane->m_normal * (distance + sphere->m_radius);
		data->penetration = -distance;

		std::stringstream ss;
		ss << sphere->name << " vs " << plane->name;
		data->result = ss.str();

		return true;
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

				PlaneCollider * plane = dynamic_cast<PlaneCollider*>(m_colliders[i]);

				if (plane)
				{
					SphereCollider * sphere = dynamic_cast<SphereCollider*>(m_colliders[j]);
					if (sphere) {
						CollisionData * pData = &data;
						if (CollisionDetector::SphereAndPlane(sphere, plane, pData)) {
							data.bodies[0] = sphere->rigidBody();
							data.bodies[1] = plane->rigidBody();
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
							data.bodies[0] = sphere->rigidBody();
							data.bodies[1] = plane->rigidBody();
							data.restitution = COF_BALL_TABLE;
							data.friction = FRICTION_BALL_CLOTH;
							collisions.push_back(data);
						}
						continue;
					}

					SphereCollider * sphere2 = dynamic_cast<SphereCollider*>(m_colliders[j]);
					if (sphere2) {
						if (SphereAndSphere(sphere, sphere2, &data)) {
							data.bodies[0] = sphere->rigidBody();
							data.bodies[1] = sphere2->rigidBody();
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