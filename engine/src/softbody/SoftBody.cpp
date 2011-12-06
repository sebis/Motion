#include "Mesh.h"
#include "MeshObject.h"
#include "SoftBody.h"
#include "Utils.h"

#include <cmath>

namespace Common
{
	SoftBody::SoftBody(GameObject * gameObject)
	{
	}

	SoftBody::~SoftBody()
	{
	}

	void SoftBody::update(float dt)
	{
		float ks = 10.0f;
		float kd = 0.22f;

		// apply forces
		for (NodeIterator nit = m_nodes.begin(); nit != m_nodes.end(); nit++)
		{
			Node * i = *nit;

			glm::vec3 gravity(0.0f, -9.81f, 0.0f);
			i->force = i->mass * gravity;
		}

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

		// TODO: proper iteration variables (maxIter, error)
		for (int i = 0; i < 30; i++)
		{
			for (SpringIterator it = m_springs.begin(); it != m_springs.end(); it++)
			{
				Node * n1 = (*it)->n1;
				Node * n2 = (*it)->n2;

				glm::vec3 p1 = n1->position;
				glm::vec3 p2 = n2->position;

				glm::vec3 delta = p2 - p1;

				float d = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
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
	}

	MeshObject * SoftBody::createCloth(Material * material, SoftBodyWorld * world)
	{
		MeshObject * object;
		{
		std::vector<Mesh::vertex> vData;
		std::vector<glm::uint> iData;

		int width = 40;
		int length = 40;

		MeshFactory::PlaneMesh(width, length, vData, iData);

		Mesh * mesh = new Mesh();

		mesh->setVertices(vData);
		mesh->setIndices(iData);

		Mesh::Vertices & vertices = mesh->vertices();

		object = new MeshObject(mesh, material);

		SoftBody * body = new SoftBody(object);

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

		for (NodeIterator it = body->m_nodes.begin(); it != body->m_nodes.end(); it++)
		{
			Node * i = *it;

			float d = i->position.x * i->position.x + i->position.z * i->position.z;
			if (d < 0.707f) {
				i->constrained = true;
			}
		}

		world->addSoftBody(body);
		}

		return object;
	}
}