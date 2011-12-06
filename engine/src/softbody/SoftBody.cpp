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
		float ks = 1.0f;
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
				// spring constant for spring ij
				
			float l_0 = s->restLength; // rest length of spring ij

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
			j->force -= fs + fv;
		}

		// integrate
		for (NodeIterator nit = m_nodes.begin(); nit != m_nodes.end(); nit++)
		{
			Node * i = *nit;

			if (i->constrained)
				continue;

			/*glm::vec3 sum(0.0f);

			for (Node::LinkIterator lit = i->links.begin(); lit != i->links.end(); lit++)
			{
				Node * j = *lit;
				sum += (glm::length(j->dv0)/glm::length(j->force)) * j->force;
			}

			int n = i->links.size();
			i->dv1 = (i->force * dt + ks * dt * dt * sum) / (i->mass + ks * dt * dt * n);

			glm::vec3 vd(0.0f);
			if (glm::length(i->velocity) > 0)
				vd = -glm::abs(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), glm::normalize(i->velocity))) * (i->velocity + i->dv1)*(1 - 1.0f/(1 + kd*glm::length(i->velocity)*glm::length(i->velocity)));*/

			//i->velocity += i->dv1 + vd;
			i->velocity += (1.0f / i->mass) * i->force * dt;
			i->position += i->velocity * dt;
		}

		// advance states
		/*for (NodeIterator nit = m_nodes.begin(); nit != m_nodes.end(); nit++)
		{
			Node * i = *nit;
			i->dv0 = i->dv1;
		}

		glm::vec3 location;*/

		// post-correct
		//if (false)
		/*for (Nodes::const_iterator nit = m_nodes.begin(); nit != m_nodes.end(); nit++)
		{
			Node * i = *nit;

			location += i->position;

			for (Node::LinkIterator lit = i->links.begin(); lit != i->links.end(); lit++)
			{*/
		float error = 0;

		static float min = 100000;
		static float max = -1;
		static float prev_d = 0;

		for (int i = 0; i < 30; i++)
		{
			//Trace::info("error: %f\n", error);
			for (SpringIterator it = m_springs.begin(); it != m_springs.end(); it++)
			{
				Node * i = (*it)->n1;
				Node * j = (*it)->n2;

				glm::vec3 p1 = i->position;
				glm::vec3 p2 = j->position;
				glm::vec3 delta = p2 - p1;

				float d = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
				float f = (*it)->restLength * (*it)->restLength;

				/*float error = (d - f) / (d + f);
				if (error < 0.0001f)
					break;*/


				float im1 = 1.0f / i->mass;
				float im2 = 1.0f / j->mass;

				//float diff = (d - f) / ((f + d) * (im1 + im2));

				if (i->constrained) {
					float diff = (d - f) / ((f + d) * im2);
					glm::vec3 offset = delta * im2 * diff;
					float ratio = glm::length(delta);
					min = std::min(min, ratio);
					max = std::max(max, ratio);
					j->position = j->position - delta * (im2 * diff);
				} else if (j->constrained) {
					float diff = (d - f) / ((f + d) * im1);
					/*glm::vec3 offset = delta * im1 * diff;
					float ratio = glm::length(offset) / glm::length(j->position);
					min = std::min(min, ratio);
					max = std::max(max, ratio);*/
					i->position = i->position + delta * (im1 * diff);
				} else {
					float diff = (d - f) / ((f + d) * (im1 + im2));
					
					/*glm::vec3 offset = delta * im1 * diff;
					float ratio = glm::length(offset) / glm::length(j->position);
					min = std::min(min, ratio);
					max = std::max(max, ratio);

					offset = delta * im2 * diff;
					ratio = glm::length(offset) / glm::length(j->position);
					min = std::min(min, ratio);
					max = std::max(max, ratio);*/

					i->position = i->position + delta * (im1 * diff);
					j->position = j->position - delta * (im2 * diff);
				}
				/*glm::vec3 ij = j->position - i->position;
				float length = glm::length(ij);
				ij = glm::normalize(ij);

				// check if stretched
				if (abs(length - 0.1f) > 0.001)
				{
					float diff = length - 0.1f;

					// do post-correct
					Trace::info("length: %f\n", length);

					// is constrained
					if (i->constrained) {
						//j->position = i->position + 0.1f * glm::normalize(ij);
						j->position -= ij * diff;

						float length2 = glm::length(j->position - i->position);
						if(length2 > length)
						{
							//Trace::info("error1\n");
						}
					} else if (j->constrained) {
						i->position += ij * diff;
					} else {
						//j->position = i->position + 0.075f * glm::normalize(ij);
						//i->position = i->position + 0.025f * glm::normalize(ij);
						j->position -= ij * (diff * 0.5f);
						i->position += ij * (diff * 0.5f);
						float length2 = glm::length(j->position - i->position);
						if (length2 > length)
						{
							//Trace::info("error2\n");
						}
					}
				}*/
			}
		}

		Trace::info("max: %f -- min: %f\n", max, min);

		//location *= (1.0f / m_nodes.size());
		//Utils::print_vector("median", location);
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