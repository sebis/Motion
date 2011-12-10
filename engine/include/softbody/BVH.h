#ifndef COMMON_BVH
#define COMMON_BVH

#include "glm/glm.hpp"

#include <list>

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
		BoundingSphere()
			: c(glm::vec3(0.0f)), r(0.0f), r2(0.0f)
		{}

		BoundingSphere(const glm::vec3 & center, float radius)
			: c(center), r(radius)
		{
			r2 = radius * radius;
		}

		inline bool isInside(const glm::vec3 & p) const;
		inline void print_debug();

		void grow(const BoundingSphere & s);

		glm::vec3 c;
		float r;
		float r2;
	};

	struct BVHNode
	{
		inline bool isInside(const glm::vec3 & p) const { return m_bv->isInside(p); }

		BoundingSphere * m_bv;

		BVHNode * left;
		BVHNode * right;

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