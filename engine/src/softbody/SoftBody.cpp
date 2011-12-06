#include "Mesh.h"
#include "MeshObject.h"
#include "SoftBody.h"
#include "Utils.h"

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
		float ks = 1.0f;
		float kd = 0.22f;

		// apply forces
		for (NodeIterator nit = m_nodes.begin(); nit != m_nodes.end(); nit++)
		{
			Node * i = *nit;

			glm::vec3 gravity(0.0f, -0.05f, 0.0f);
			i->force = i->mass * gravity;

			for (Node::LinkIterator lit = i->links.begin(); lit != i->links.end(); lit++)
			{
				Node * j = *lit;

				  // spring constant for spring ij
				
				float l_0 = 0.1f; // rest length of spring ij

				/*glm::vec3 e = j->position - i->position;
				float l = glm::length(e);
				e = glm::normalize(e);

				float vi = glm::dot(e, i->velocity);
				float vj = glm::dot(e, j->velocity);

				float f_sd = -ks*(l_0 - l) - kd*(vi - vj);
				i->force += f_sd * e;*/

				//Utils::print_vector("additional", f_sd * e);

				glm::vec3 fs = ks*(glm::length(j->position - i->position) - l_0) * glm::normalize(j->position - i->position);
				glm::vec3 fv = ks*(j->velocity - i->velocity);
				i->force += fs + fv;
			}
		}

		// integrate
		for (NodeIterator nit = m_nodes.begin(); nit != m_nodes.end(); nit++)
		{
			Node * i = *nit;

			glm::vec3 sum(0.0f);

			for (Node::LinkIterator lit = i->links.begin(); lit != i->links.end(); lit++)
			{
				Node * j = *lit;
				sum += (glm::length(j->dv0)/glm::length(j->force)) * j->force;
			}

			int n = i->links.size();
			i->dv1 = (i->force * dt + ks * dt * dt * sum) / (i->mass + ks * dt * dt * n);

			glm::vec3 vd(0.0f);
			/*if (glm::length(i->velocity) > 0)
				vd = -glm::abs(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), glm::normalize(i->velocity))) * (i->velocity + i->dv1)*(1 - 1.0f/(1 + kd*glm::length(i->velocity)*glm::length(i->velocity)));*/

			float d = glm::sqrt(i->position.x * i->position.x + i->position.z * i->position.z);
			if (true) {
			//if (d > 0.01f) {
				i->velocity += i->dv1 + vd;
				i->position += i->velocity * dt;
			} else {
				i->dv1 = glm::vec3(0.0f);
			}
		}

		// advance states
		for (NodeIterator nit = m_nodes.begin(); nit != m_nodes.end(); nit++)
		{
			Node * i = *nit;
			i->dv0 = i->dv1;
		}

		glm::vec3 location;

		// post-correct
		if (false)
		for (NodeIterator nit = m_nodes.begin(); nit != m_nodes.end(); nit++)
		{
			Node * i = *nit;

			location += i->position;

			for (Node::LinkIterator lit = i->links.begin(); lit != i->links.end(); lit++)
			{
				Node * j = *lit;

				glm::vec3 ij = j->position - i->position;
				float length = glm::length(ij);
				ij = glm::normalize(ij);

				// check if stretched
				if (abs(length - 0.1f) > 0.001)
				{
					float diff = length - 0.1f;

					// do post-correct
					//Trace::info("length: %f\n", length);

					float di = glm::sqrt(i->position.x * i->position.x + i->position.z * i->position.z);
					float dj = glm::sqrt(j->position.x * j->position.x + j->position.z * j->position.z);

					// is constrained
					if (di < 0.01f) {
						//j->position = i->position + 0.1f * glm::normalize(ij);
						j->position -= ij * diff;

						float length2 = glm::length(j->position - i->position);
						if(length2 > length)
						{
							//Trace::info("error1\n");
						}
					} else if (dj < 0.01f) {
						i->position -= ij * diff;
					} else {
						//j->position = i->position + 0.075f * glm::normalize(ij);
						//i->position = i->position + 0.025f * glm::normalize(ij);
						j->position -= ij * (diff * 0.5f);
						i->position -= ij * (diff * 0.5f);
						float length2 = glm::length(j->position - i->position);
						if (length2 > length)
						{
							//Trace::info("error2\n");
						}
					}
				}
			}
		}

		location *= (1.0f / m_nodes.size());
		Utils::print_vector("median", location);
	}

	MeshObject * SoftBody::createCloth(Material * material, SoftBodyWorld * world)
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

		// construct links
		for (int z = 0; z < length; z++)
		{
			for (int x = 0; x < width; x++)
			{
				int index = (width+1)*z + x;
				Node * node = body->m_nodes[index];

				if (x > 0)
					node->links.insert(body->m_nodes[z*(width+1) + (x-1)]);
				if (x < width)
					node->links.insert(body->m_nodes[z*(width+1) + (x+1)]);
				if (z > 0)
					node->links.insert(body->m_nodes[(z-1)*(width+1) + x]);
				if (z < length)
					node->links.insert(body->m_nodes[(z+1)*(width+1) + x]);
				if (x > 0 && z > 0)
					node->links.insert(body->m_nodes[(z-1)*(width+1) + (x-1)]);
				if (x < width && z < length)
					node->links.insert(body->m_nodes[(z+1)*(width+1) + (x+1)]);

			}
		}

		world->addSoftBody(body);
		}

		return object;
	}
}