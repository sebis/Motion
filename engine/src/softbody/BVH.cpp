#include "BVH.h"
#include "Mesh.h"

namespace Common
{
	// Helper functions
	namespace
	{
		Triangle * createTriangle(const Mesh::vertex & a, const Mesh::vertex & b, const Mesh::vertex & c)
		{
			Triangle * t = new Triangle;

			t->a = a.position;
			t->b = b.position;
			t->c = c.position;

			t->ab = t->b - t->a;
			t->ac = t->c - t->a;
			t->bc = t->c - t->b;

			t->n = glm::cross(t->ab, t->ac);

			return t;
		}

		BoundingSphere * fromTriangle(const Triangle * t)
		{
			std::pair<const glm::vec3 *, const glm::vec3 *> max;
			float maxDistanceSqr = std::numeric_limits<float>::min();

			float d_ab = glm::dot(t->ab, t->ab);
			float d_ac = glm::dot(t->ac, t->ac);
			float d_bc = glm::dot(t->bc, t->bc);

			if (d_ab > maxDistanceSqr) {
				maxDistanceSqr = d_ab;
				max.first = &t->a;
				max.second = &t->b;
			}
			if (d_ac > maxDistanceSqr) {
				maxDistanceSqr = d_ac;
				max.first = &t->a;
				max.second = &t->c;
			}
			if (d_bc > maxDistanceSqr) {
				max.first = &t->b;
				max.second = &t->c;
			}

			assert(max.first && max.second);

			glm::vec3 center = *max.first + 0.5f * (*max.second - *max.first);
			float radius = 0.5f * maxDistanceSqr;

			return new BoundingSphere(center, radius);
		}
	}

	bool BoundingSphere::isInside(const glm::vec3 & p) const
	{
		return glm::distance(p, m_center) < m_radius;
	}

	void BoundingSphere::grow(const BoundingSphere & s)
	{
		const glm::vec3 & d = m_center - s.m_center;
		float dist2 = glm::dot(d, d);

		if (glm::sqrt(s.m_radius - m_radius) >= dist2) {
			if (s.m_radius >= m_radius) {
				m_radius = s.m_radius;
				m_center = s.m_center;
			}
		} else {
			float dist = glm::sqrt(dist2);
			float newRadius = (dist + m_radius + s.m_radius) * 0.5f;
			if (dist > std::numeric_limits<float>::epsilon())
				m_center += ((newRadius - m_radius) / dist) * d;
			m_radius = newRadius;
		}
	}

	BVH * BVH::constructFromMesh(Mesh * mesh)
	{
		const Mesh::Indices & indices = mesh->indices();

		std::vector<BVHNode*> leaves;

		BoundingSphere * rootSphere;

		for (unsigned i = 0; i < indices.size(); i += 3)
		{
			const Mesh::vertex & a = mesh->vertexAt(indices[i+0]);
			const Mesh::vertex & b = mesh->vertexAt(indices[i+1]);
			const Mesh::vertex & c = mesh->vertexAt(indices[i+2]);

			Triangle * t = createTriangle(a, b, c);

			BoundingSphere * bs = fromTriangle(t);

			BVHNode * node = new BVHNode;
			node->m_triangle = t;
			node->m_bv = bs;
			node->m_isLeaf = true;

			leaves.push_back(node);
		}

		rootSphere = new BoundingSphere(*static_cast<BoundingSphere *>(leaves[0]->m_bv));
		for (int i = 1; i < leaves.size(); i++)
			rootSphere->grow(*static_cast<BoundingSphere *>(leaves[i]->m_bv));

		BVHNode * root = new BVHNode();
		root->m_bv = rootSphere;
		root->m_isLeaf = false;
		root->m_children = leaves;

		BVH * bvh = new BVH();
		bvh->m_root = root;

		return bvh;
	}
}