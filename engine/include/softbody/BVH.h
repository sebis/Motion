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
		virtual void print_debug() {}
	};

	class BoundingSphere : public BoundingVolume
	{
	public:
		BoundingSphere(const glm::vec3 & center, float radius)
			: m_center(center), m_radius(radius)
		{
			m_radius2 = radius * radius;
		}

		inline bool isInside(const glm::vec3 & p) const;
		inline void print_debug();

		void grow(const BoundingSphere & s);

		const glm::vec3 & center() const { return m_center; }
		float radius() const { return m_radius; }

	private:
		glm::vec3 m_center;
		float m_radius;
		float m_radius2;
	};

	struct BVHNode
	{
		inline bool isInside(const glm::vec3 & p) { return m_bv->isInside(p); }

		BoundingVolume * m_bv;
		std::vector<BVHNode*> m_children;

		Triangle * m_triangle;
		bool m_isLeaf;
	};

	class BVH
	{
	public:
		static BVH * constructFromMesh(Mesh * mesh);

		inline BVHNode * root() const { return m_root; }

	private:
		friend class CollisionDetector;

		BVHNode * m_root;
	};
}

#endif /* COMMON_BVH */