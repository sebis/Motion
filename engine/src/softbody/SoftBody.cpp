#include "CollisionDetector.h"
#include "Mesh.h"
#include "MeshObject.h"
#include "SoftBody.h"

#include <cmath>

namespace Common
{
	// Helper functions
	namespace
	{
		void addForceToTriangle(SoftBody::Node * n1, SoftBody::Node * n2, SoftBody::Node * n3, const glm::vec3 & force)
		{
			const glm::vec3 & n12 = n2->position - n1->position;
			const glm::vec3 & n13 = n3->position - n1->position;

			const glm::vec3 n = glm::normalize(glm::cross(n12, n13));

			const glm::vec3 directedForce = n * glm::dot(n, force);

			n1->force += directedForce;
			n2->force += directedForce;
			n3->force += directedForce;
		}
	}

	const unsigned SoftBody::WIDTH = 50;
	const unsigned SoftBody::LENGTH = 50;

	SoftBody::SoftBody(GameObject * gameObject)
		: m_gameObject(gameObject)
	{
	}

	SoftBody::~SoftBody()
	{
	}

	void SoftBody::applyForces(float dt)
	{
		float dt2 = dt * dt;

		const glm::vec3 gravity(0.0f, -9.81f, 0.0f);
		const glm::vec3 wind(0.5f, 1.0f, 0.1f);

		for (int z = 0; z < LENGTH; z++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				Node * n1 = node(x, z);

				n1->force += gravity * dt2;

				if (x < WIDTH - 1 && z < LENGTH - 1) {
					Node * n2 = node(x, z+1);
					Node * n3 = node(x+1, z);
					Node * n4 = node(x+1, z+1);

					addForceToTriangle(n1, n2, n3, wind * dt2);
					addForceToTriangle(n3, n2, n4, wind * dt2);
				}
			}
		}
	}

	void SoftBody::solveConstraints()
	{
		const unsigned maxIter = 5;

		SpringIterator begin = m_springs.begin();
		SpringIterator end = m_springs.end();

		for (unsigned i = 0; i < maxIter; i++)
		{
			for (SpringIterator it = begin; it != end; it++)
			{
				Node * n1 = (*it)->n1;
				Node * n2 = (*it)->n2;

				const glm::vec3 & delta = n2->position - n1->position;

				float d = glm::length(delta);
				float l = (*it)->restLength;

				const glm::vec3 & offset = delta * (1 - l/d) * 0.5f;

				n1->position += offset;
				n2->position -= offset;

				/*float d = glm::dot(delta, delta);
				float f = (*it)->restLength * (*it)->restLength;

				float im1 = n1->constrained ? 0 : 1.0f / n1->mass;
				float im2 = n2->constrained ? 0 : 1.0f / n2->mass;

				float diff = (d - f) / ((f + d) * (im1 + im2));

				if (im1 != 0) {
					n1->position += delta * (im1 * diff);
				}

				if (im2 != 0) {
					n2->position -= delta * (im2 * diff);
				}*/
			}
		}
	}

	void SoftBody::integrate(float dt)
	{
		float dt2 = dt * dt;

		glm::vec3 velocity;
		glm::vec3 tmp;
		float damping = 0.99f;

		for (NodeIterator nit = m_nodes.begin(); nit != m_nodes.end(); nit++)
		{
			Node * i = *nit;

			if (i->constrained)
				continue;

			// Use Verlet integration to calculate velocity and new position based on previous state
			tmp = i->position;
			velocity = (i->position - i->oldPosition) * damping + (i->force / i->mass) * dt2;
			i->position += velocity;
			i->oldPosition = tmp;

			i->force = glm::vec3(0.0f);
		}
	}

	void SoftBody::resolveCollisions()
	{
		const glm::mat4 & world = m_gameObject->transform().world();
		CollisionDetector * cd = CollisionDetector::instance();

		for (NodeIterator nit = m_nodes.begin(); nit != m_nodes.end(); nit++)
		{
			Node * i = *nit;
			
			glm::vec3 wp1 = glm::vec3(world * glm::vec4(i->position, 1.0f));
			glm::vec3 dummy;

			Contact * contact = cd->collides(wp1, dummy);
			if (contact) {
				float friction = 5.0f;

				glm::vec3 rn = contact->point - wp1;
				glm::vec3 d = i->position - i->oldPosition;
				glm::vec3 dt = d - contact->normal * glm::dot(d, contact->normal);
				glm::vec3 rt = -friction * dt;

				i->position += rn + rt;
			}
		}
	}

	void SoftBody::update(float dt)
	{
		applyForces(dt);
		solveConstraints();
		integrate(dt);
		resolveCollisions();
		//calculateNormals();
	}

	SoftBody::Node * SoftBody::node(unsigned x, unsigned z)
	{
		return m_nodes[z*WIDTH + x];
	}

	MeshObject * SoftBody::createCloth(Material * material, SoftBodyWorld * world, SoftBody * body)
	{
		std::vector<Mesh::vertex> vData;
		std::vector<glm::uint> iData;

		int width = WIDTH;
		int length = LENGTH;

		MeshFactory::PlaneMesh(width-1, length-1, vData, iData);

		Mesh * mesh = new Mesh();

		mesh->setVertices(vData);
		mesh->setIndices(iData);

		Mesh::Vertices & vertices = mesh->vertices();

		MeshObject * object = new MeshObject(mesh, material);

		body = new SoftBody(object);

		// initialize nodes
		for (int z = 0; z < length; z++)
		{
			for (int x = 0; x < width; x++)
			{
				int index = z*width + x;

				Mesh::vertex * v = &vertices[index];
				Node * node = new Node(v->position, 0.000001f);

				body->m_nodes.push_back(node);
			}
		}

		assert(body->m_nodes.size() == vData.size());

		// Construct links
		for (int z = 0; z < length; z++)
		{
			for (int x = 0; x < width; x++)
			{
				Node * n1 = body->node(x, z);

				// Immediate neighbors
				if (x < width - 1) {
					Node * n2  = body->node(x+1, z);
					body->m_springs.push_back(new Spring(n1, n2));
				}
				if (z < length - 1) {
					Node * n2 = body->node(x, z+1);
					body->m_springs.push_back(new Spring(n1, n2));
				}
				if (x < width - 1 && z < length - 1) {
					Node * n2 = body->node(x+1, z+1);
					body->m_springs.push_back(new Spring(n1, n2));
				}

				// Secondary neighbors
				if (x < width - 2) {
					Node * n2 = body->node(x+2, z);
					body->m_springs.push_back(new Spring(n1, n2));
				}
				if (z < length - 2) {
					Node * n2 = body->node(x, z+2);
					body->m_springs.push_back(new Spring(n1, n2));
				}
				if (x < width - 2 && z < length - 2) {
					Node * n2 = body->node(x+2, z+2);
					body->m_springs.push_back(new Spring(n1, n2));
				}
			}
		}

		world->addSoftBody(body);

		return object;
	}
}