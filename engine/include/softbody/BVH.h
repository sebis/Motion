#ifndef COMMON_BVH
#define COMMON_BVH

#include "glm/glm.hpp"

#include <vector>

namespace Common
{
	class Mesh;

	struct Triangle
	{
		glm::vec3 a, b, c;
		glm::vec3 n;

		// precomputed edges
		glm::vec3 ab, ac, bc;
	};

	class BoundingVolume
	{
	public:
		virtual bool isInside(const glm::vec3 & p) const = 0;
	};

	class BoundingSphere : public BoundingVolume
	{
	public:
		BoundingSphere(const glm::vec3 & center, float radius)
			: m_center(center), m_radius(radius) {}

		virtual bool isInside(const glm::vec3 & p) const;
		void grow(const BoundingSphere & s);

	private:
		glm::vec3 m_center;
		float m_radius;
	};

	struct BVHNode
	{
		BoundingVolume * m_bv;
		std::vector<BVHNode*> m_children;

		Triangle * m_triangle;
		bool m_isLeaf;
	};

	class BVH
	{
	public:
		bool isInside(const glm::vec3 & p) const;

		static BVH * constructFromMesh(Mesh * mesh);

	private:
		friend class CollisionDetector;

		BVHNode * m_root;
	};
}

#endif /* COMMON_BVH */