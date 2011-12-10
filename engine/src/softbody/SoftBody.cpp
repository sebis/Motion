#include "CollisionDetector.h"
#include "Mesh.h"
#include "MeshObject.h"
#include "SoftBody.h"
#include "Utils.h"

#include <cmath>

namespace Common
{
	SoftBody::SoftBody(GameObject * gameObject)
		: m_gameObject(gameObject)
	{
	}

	SoftBody::~SoftBody()
	{
	}

	void SoftBody::update(float dt)
	{
		float ks = 10.0f;
		float kd = 0.22f;

		// apply gravity forces
		for (NodeIterator nit = m_nodes.begin(); nit != m_nodes.end(); nit++)
		{
			Node * i = *nit;

			glm::vec3 gravity(0.0f, -0.981f, 0.0f);
			i->force = i->mass * gravity;
		}

		// apply internal forces
		for (SpringIterator it = m_springs.begin(); it != m_springs.end(); it++)
		{
			Spring * s = *it;

			Node * i = s->n1;
			Node * j = s->n2;
				
			float l0 = s->restLength; // rest length of spring ij

			glm::vec3 fs = ks*(glm::length(j->position - i->position) - l0) * glm::normalize(j->position - i->position);
			glm::vec3 fv = ks*(j->velocity - i->velocity);

			i->force += fs + fv;
			j->force -= fs + fv;
		}

		// integrate
		for (NodeIterator nit = m_nodes.begin(); nit != m_nodes.end(); nit++)
		{
			Node * i = *nit;

			if (i->constrained)
				continue;

			i->velocity += (1.0f / i->mass) * i->force * dt;
			i->position += i->velocity * dt;
		}

		// post-process inverse dynamics
		glm::mat4 world = m_gameObject->transform().world();
		glm::mat4 invWorld = glm::inverse(m_gameObject->transform().world());

		CollisionDetector * cd = CollisionDetector::instance();
		// TODO: proper iteration variables (maxIter, error)
		for (int i = 0; i < 1; i++)
		{
			for (SpringIterator it = m_springs.begin(); it != m_springs.end(); it++)
			{
				Node * n1 = (*it)->n1;
				Node * n2 = (*it)->n2;

				if (!n1->constrained) {
					glm::vec3 wp1 = glm::vec3(world * glm::vec4(n1->position, 1.0f));
					glm::vec3 wp2 = glm::vec3(world * glm::vec4(n1->position + n1->velocity * dt, 1.0f));

					Contact * contact = cd->collides(wp1, wp2);
					if (contact) {
						//glm::vec3 rn = glm::vec3(invWorld * glm::vec4(contact->point, 1.0f));
						n1->position += glm::dot(contact->normal, contact->point - wp1) * contact->normal;
						//n1->position += contact->normal * contact->penetration;
					}
				}

				if (!n2->constrained) {
					glm::vec3 wp1 = glm::vec3(world * glm::vec4(n2->position, 1.0f));
					glm::vec3 wp2 = glm::vec3(world * glm::vec4(n2->position + n2->velocity * dt, 1.0f));

					Contact * contact = cd->collides(wp1, wp2);
					if (contact) {
						//glm::vec3 rn = glm::vec3(invWorld * glm::vec4(contact->point, 1.0f));
						n2->position += glm::dot(contact->normal, contact->point - wp1) * contact->normal;
						//n2->position += contact->normal * contact->penetration;
					}
				}

				glm::vec3 p1 = n1->position;
				glm::vec3 p2 = n2->position;

				const glm::vec3 & delta = p2 - p1;

				float d = glm::dot(delta, delta);
				float f = (*it)->restLength * (*it)->restLength;

				float im1 = n1->constrained ? 0 : 1.0f / n1->mass;
				float im2 = n2->constrained ? 0 : 1.0f / n2->mass;

				float diff = (d - f) / ((f + d) * (im1 + im2));

				if (im1 != 0) {
					n1->position += delta * (im1 * diff);
				}

				if (im2 != 0) {
					n2->position -= delta * (im2 * diff);
				}
			}
		}

		/*if (false)
		for (NodeIterator nit = m_nodes.begin(); nit != m_nodes.end(); nit++)
		{
			Node * i = *nit;

			if (i->constrained)
				continue;

			glm::vec3 newVelocity = (1.0f / i->mass) * i->force * dt;
			glm::vec3 newPosition = i->position + newVelocity * dt;
			Contact * contact = CollisionDetector::instance()->collides(glm::vec3(m_gameObject->transform().world() * glm::vec4(i->position, 1.0f)));

			if (contact) {
				//i->constrained = true;
				//glm::vec3 rn = contact->point;
				//glm::vec3 d = i->velocity * dt;
				//glm::vec3 dt = d - contact->normal * glm::dot(d, contact->normal);
				//glm::vec3 rt = -1.0f * dt;

				////i->position += (rn + rt);
				//i->position += rn;
				//i->velocity = glm::vec3(0.0f);

				assert(glm::length(contact->normal) < 1.1f && glm::length(contact->normal) > 0.9f);
				glm::vec3 vn = glm::dot(i->velocity, contact->normal) * contact->normal;
				glm::vec3 vt = i->velocity - vn;

				float kf = 0.1f;
				float kd = 1.0f;

				if (glm::length(vt) >= kf * glm::length(vn)) {
					Utils::print_vector("vt", vt);
					i->velocity = vt - kf * glm::length(vn) * glm::normalize(vt) - kd * vn;
				} else {

					Utils::print_vector("vn", vn);
					i->velocity = -kd * vn;
				}
				
				i->position += i->velocity * dt;
			}
		}*/
	}

	MeshObject * SoftBody::createCloth(Material * material, SoftBodyWorld * world, SoftBody * body)
	{
		MeshObject * object;
		{
		std::vector<Mesh::vertex> vData;
		std::vector<glm::uint> iData;

		int width = 20;
		int length = 20;

		MeshFactory::PlaneMesh(width, length, vData, iData);

		Mesh * mesh = new Mesh();

		mesh->setVertices(vData);
		mesh->setIndices(iData);

		Mesh::Vertices & vertices = mesh->vertices();

		object = new MeshObject(mesh, material);

		body = new SoftBody(object);

		// initialize nodes
		for (int z = 0; z < length+1; z++)
		{
			for (int x = 0; x < width+1; x++)
			{
				int index = (width+1)*z + x;

				Mesh::vertex * v = &vertices[index];
				Node * node = new Node(v->position, 0.1f);

				body->m_nodes.push_back(node);
			}
		}

		assert(body->m_nodes.size() == vData.size());

		// construct links
		for (int z = 0; z < length+1; z++)
		{
			for (int x = 0; x < width+1; x++)
			{
				int index = (width+1)*z + x;
				Trace::info("%d, ", index);
				Node * node = body->m_nodes[index];

				if (x < width) {
					Spring * s = new Spring(node, body->m_nodes[z*(width+1) + (x+1)], 1.0f / width);
					body->m_springs.push_back(s);
				}

				if (z < length) {
					Spring * s = new Spring(node, body->m_nodes[(z+1)*(width+1) + x], 1.0f / length);
					body->m_springs.push_back(s);
				}

				if (x < width && z < length) {
					Spring * s = new Spring(node, body->m_nodes[(z+1)*(width+1) + (x+1)], sqrt((1.0f / length) * (1.0f / length) + (1.0f / width) * (1.0f / width)));
					body->m_springs.push_back(s);
				}

				if (x < width && z < length) {
					Node * n1 = body->m_nodes[z*(width+1) + (x+1)];
					Node * n2 = body->m_nodes[(z+1)*(width+1) + x];
					Spring * s = new Spring(n1, n2, sqrt((1.0f / length) * (1.0f / length) + (1.0f / width) * (1.0f / width)));
					body->m_springs.push_back(s);
				}

				if (x > 0)
					node->links.insert(body->m_nodes[z*(width+1) + (x-1)]);
				if (x < width)
					node->links.insert(body->m_nodes[z*(width+1) + (x+1)]);
				if (z > 0)
					node->links.insert(body->m_nodes[(z-1)*(width+1) + x]);
				if (z < length)
					node->links.insert(body->m_nodes[(z+1)*(width+1) + x]);

			}
		}

		// for testing only
		/*for (NodeIterator it = body->m_nodes.begin(); it != body->m_nodes.end(); it++)
		{
			Node * i = *it;

			float d = i->position.x * i->position.x + i->position.z * i->position.z;
			if (d < 0.707f) {
				i->constrained = true;
			}
		}*/

		world->addSoftBody(body);
		}

		return object;
	}
}