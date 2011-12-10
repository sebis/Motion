#include "BVH.h"
#include "CollisionDetector.h"
#include "Trace.h"

#include <sstream>

namespace
{
	// From Real-Time Collision Detection by Christopher Ericson. Uses Lagrange's identity.
	bool PointInTriangle(const glm::vec3 & p, const glm::vec3 & a, const glm::vec3 & b, const glm::vec3 & c, glm::vec3 & n, glm::vec3 & q, float & t)
	{
		const glm::vec3 & pa = a - p;
		const glm::vec3 & pb = b - p;
		const glm::vec3 & pc = c - p;

		const glm::vec3 u = glm::cross(pb, pc);
		const glm::vec3 v = glm::cross(pc, pa);

		if (glm::dot(u, v) < 0.0f)
			return false;

		const glm::vec3 w = glm::cross(pa, pb);
		
		if (glm::dot(u, w) < 0.0f)
			return false;

		const glm::vec3 & ab = b - a;
		const glm::vec3 & ac = c - a;

		// calculate normal for triangle
		n = glm::normalize(glm::cross(ab, ac));
		//n *= 1.0f / glm::dot(n, n);

		// distance of point p along normal
		t = glm::dot(n, p - a);
		const float CONSTANT = 0.05f;
		if (t > CONSTANT)
			return false;

		// projected point on triangle
		q = p + n * (CONSTANT - t);

		return true;
	}

	bool PointInBVH(const Common::BVH * bvh, const glm::vec3 & p, Common::Contact * contact)
	{
		Common::BVHNode * parent = bvh->root();

		if (parent->isInside(p)) {

			for (std::vector<Common::BVHNode*>::iterator it = parent->m_children.begin(); it != parent->m_children.end(); it++)
			{
				Common::BVHNode * current = *it;
				if (current->isInside(p)) {
					Common::Triangle * tri = current->m_triangle;
					glm::vec3 n, q;
					float t;
					if (PointInTriangle(p, tri->a, tri->b, tri->c, n, q, t))
					{
						contact->normal = n;
						contact->point = q;
						contact->penetration = t;

						return true;
					}
				}
			}
		}

		return false;
	}

	bool IntersectLineTriangle(const glm::vec3 & p, const glm::vec3 & q, const glm::vec3 & a, const glm::vec3 & b, const glm::vec3 & c, glm::vec3 & n, glm::vec3 & r, float & t)
	{
		const glm::vec3 & ab = b - a;
		const glm::vec3 & ac = c - a;
		const glm::vec3 & qp = p - q;

		n = glm::cross(ab, ac);

		float d = glm::dot(qp, n);
		if (d <= 0.0f)
			return false;

		const glm::vec3 & ap = p - a;

		t = glm::dot(ap, n);
		if (t < -0.05f || t > d)
			return false;

		const glm::vec3 e = glm::cross(qp, ap);

		float v = glm::dot(ac, e);
		if (v < 0.0f || v > d)
			return false;

		float w = -glm::dot(ab, e);
		if (w < 0.0f || v + w > d)
			return false;

		float ood = 1.0f / d;
		
		t *= ood;

		v *= ood;
		w *= ood;
		float u = 1.0f - v - w;

		//r = p + t * (q - p);
		r = u * a + v * b + w * c;

		return true;
	}
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

	bool CollisionDetector::MeshAndMesh(MeshCollider * mesh1, MeshCollider * mesh2, CollisionData * data)
	{
		if (mesh1->m_usePoints && !mesh2->m_usePoints) {
			// right order, do nothing
		} else if (mesh2->m_usePoints && !mesh1->m_usePoints) {
			// swap
			MeshCollider * tmp = mesh1;
			mesh1 = mesh2;
			mesh2 = tmp;
		} else {
			Trace::error("Unsupported mesh collision\n");
			return false;
		}

		const glm::mat4 & world1 = mesh1->transform().world();
		const glm::mat4 & world2 = mesh2->transform().world();

		// check if a points are contained in b's triangles
		Mesh::Indices indices = mesh2->m_mesh->indices();

		for (int i = 0; i < indices.size(); i += 3) {

			const glm::vec3 a(world2 * glm::vec4(mesh2->m_mesh->vertexAt(indices[i+0]).position, 1.0f));
			const glm::vec3 b(world2 * glm::vec4(mesh2->m_mesh->vertexAt(indices[i+1]).position, 1.0f));
			const glm::vec3 c(world2 * glm::vec4(mesh2->m_mesh->vertexAt(indices[i+2]).position, 1.0f));

			glm::vec3 finalNormal;
			glm::vec3 finalPoint;
			float minPenetration = std::numeric_limits<float>::max();
			bool collide = false;

			int totalcounter = 0;
			int incounter = 0;
			int hitcounter = 0;

			for (int v = 0; v < mesh1->m_mesh->vertices().size(); v++)
			{
				totalcounter++;

				glm::vec3 * currentVertex = &mesh1->m_mesh->vertexAt(v).position;
				const glm::vec3 p(world1 * glm::vec4(*currentVertex, 1.0f));

				glm::vec3 normal;
				glm::vec3 point;
				float penetration;

				if (PointInTriangle(p, a, b, c, normal, point, penetration))
				{
					incounter++;
					if (penetration < minPenetration)
					{
						Contact * contact = new Contact;

						contact->normal = normal;
						contact->point = point;
						contact->penetration = penetration;
						contact->userData = currentVertex;

						data->contacts.push_back(contact);
					}
				}
			}
		}

		Trace::info("Number of contacts: %d\n", data->contacts.size());
		return data->contacts.size() > 0;
	}

	void getVertices(MeshCollider * mesh, const Mesh::Indices & indices, glm::vec3 & a, glm::vec3 & b, glm::vec3 & c, int i)
	{
		a = mesh->m_mesh->vertexAt(indices[i+0]).position;
		b = mesh->m_mesh->vertexAt(indices[i+1]).position;
		c = mesh->m_mesh->vertexAt(indices[i+2]).position;
	}

	Contact * collidesInternal(MeshCollider * mesh, const glm::vec3 & p, const glm::vec3 & q, const Mesh::Indices & indices)
	{
		glm::vec3 finalNormal(0.0f);
		glm::vec3 finalPoint(0.0f);
		float minPenetration = std::numeric_limits<float>::max();
		bool collide = false;

		int count = indices.size();

		glm::vec3 normal(0.0f);
		glm::vec3 point(0.0f);
		float penetration = 0;

		for (int i = 0; i < count; i += 3) {

			glm::vec3 a, b, c;
			getVertices(mesh, indices, a, b, c, i);

			if (PointInTriangle(p, a, b, c, normal, point, penetration))
			{
				if (penetration < minPenetration)
				{
					finalNormal = normal;
					finalPoint = point;
					minPenetration = penetration;
					collide = true;
				}
			}

			/*if (IntersectLineTriangle(p, q, a, b, c, normal, point, penetration))
			{
				Contact * contact = new Contact;

				contact->normal = normal;
				contact->point = point;
				contact->penetration = penetration;

				return contact;
			}*/
		}

		if (collide) 
		{
			Contact * contact = new Contact;

			contact->normal = finalNormal;
			contact->point = finalPoint;
			contact->penetration = minPenetration;

			return contact;
		}

		return 0;
	}

	Contact * CollisionDetector::collides(const glm::vec3 & p1, const glm::vec3 & p2)
	{
		Contact * contact = new Contact;

		for (unsigned i = 0; i < m_colliders.size(); i++)
		{
			MeshCollider * mesh = dynamic_cast<MeshCollider*>(m_colliders[i]);
			if (mesh)
			{
				const glm::mat4 & world = mesh->transform().world();
				const glm::mat4 & invWorld = glm::inverse(world);
				const glm::vec3 & p = glm::vec3(invWorld * glm::vec4(p1, 1.0f));
				const glm::vec3 & q = glm::vec3(invWorld * glm::vec4(p2, 1.0f));

				/*const Mesh::Indices & indices = mesh->m_mesh->indices();

				Contact * contact = collidesInternal(mesh, p, q, indices);
				if (contact) {

					contact->normal = glm::vec3(world * glm::vec4(contact->normal, 0.0f));
					contact->point = glm::vec3(world * glm::vec4(contact->point, 1.0f));

					return contact;
				}*/

				if (PointInBVH(mesh->m_bvh, p, contact))
				{
					contact->normal = glm::vec3(world * glm::vec4(contact->normal, 0.0f));
					contact->point = glm::vec3(world * glm::vec4(contact->point, 1.0f));

					return contact;
				}
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

				MeshCollider * mesh = dynamic_cast<MeshCollider*>(m_colliders[i]);
				if (mesh)
				{
					MeshCollider * mesh2 = dynamic_cast<MeshCollider*>(m_colliders[j]);
					if (mesh2) {
						CollisionData * pData = &data;
						if (CollisionDetector::MeshAndMesh(mesh, mesh2, pData)) {
							data.bodies[0] = mesh->collisionBody();
							data.bodies[1] = mesh->collisionBody();
							data.restitution = 0;
							data.friction = 0;
							collisions.push_back(data);
						}
						continue;
					}
					continue; // Mesh-Other not implemented
				}

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