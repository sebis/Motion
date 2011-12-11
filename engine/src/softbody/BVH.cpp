#include "BVH.h"
#include "Mesh.h"
#include "Trace.h"

#include <list>

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

			t->n = glm::normalize(glm::cross(t->ab, t->ac));

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
			float radius = 0.5f * glm::sqrt(maxDistanceSqr);

			return new BoundingSphere(center, radius);
		}

		typedef std::list<BVHNode*>::iterator NodeIterator;

		float ComputeVolume(const BoundingSphere & s)
		{
			return (4.0f / 3.0f) * M_PI * s.r * s.r * s.r;
		}

		void ComputeBoundingVolume(BoundingSphere & s, const BoundingSphere s0, const BoundingSphere s1)
		{
			glm::vec3 d = s1.c - s0.c;
			float dist2 = glm::dot(d, d);

			if ((s1.r - s0.r) * (s1.r - s0.r) >= dist2) {
				if (s1.r >= s0.r)
					s = s1;
				else
					s = s0;
			} else {
				float dist = std::sqrt(dist2);
				s.r = (dist + s0.r + s1.r) * 0.5f;
				s.r2 = s.r * s.r;
				s.c = s0.c;
				if (dist > std::numeric_limits<float>::epsilon())
					s.c += ((s.r - s0.r) / dist) * d;
			}
		}

		BoundingSphere * FindNodesToMerge(std::list<BVHNode *> & nodes, NodeIterator & i, NodeIterator & j)
		{
			NodeIterator begin = nodes.begin();
			NodeIterator end = nodes.end();

			BoundingSphere s, best;

			float smallestVolume = std::numeric_limits<float>::max();

			Trace::info("num nodes: %d\n", nodes.size());

			for (NodeIterator it = begin; it != end; it++)
			{
				NodeIterator it2(it);
				for (it2++; it2 != end; it2++)
				{
					ComputeBoundingVolume(s, *(*it)->m_bv, *(*it2)->m_bv);

					float volume = ComputeVolume(s);

					if (volume < smallestVolume) {
						smallestVolume = volume;

						best = s;

						i = it;
						j = it2;
					}
				}
			}

			return new BoundingSphere(best);
		}

		BVHNode * BottomUpBVTree(std::list<BVHNode*> & nodes)
		{
			NodeIterator i, j;

			while (nodes.size() > 1)
			{
				BoundingSphere * s = FindNodesToMerge(nodes, i, j);

				BVHNode * node = new BVHNode;

				node->m_isLeaf = false;
				node->left = *i;
				node->right = *j;
				node->m_bv = s;

				nodes.erase(i);
				nodes.erase(j);

				nodes.push_back(node);
			}

			return nodes.front();
		}
	}

	bool BoundingSphere::isInside(const glm::vec3 & p) const
	{
		const glm::vec3 & diff = p - c;
		return glm::dot(diff, diff) < r2;
	}

	void BoundingSphere::print_debug()
	{
		Trace::info("Bounding sphere: (%.2f, %.2f, %.2f) -- %.2f\n", c.x, c.y, c.z, r);
	}

	BVH * BVH::constructFromMesh(Mesh * mesh)
	{
		const Mesh::Indices & indices = mesh->indices();
		std::list<BVHNode*> nodes;

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
			node->left = 0;
			node->right = 0;

			nodes.push_back(node);
		}

		BVH * bvh = new BVH();
		bvh->m_root = BottomUpBVTree(nodes);

		return bvh;
	}
}