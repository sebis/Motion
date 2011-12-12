#ifndef COMMON_COLLIDER
#define COMMON_COLLIDER

#include "BVH.h"
#include "CollisionBody.h"
#include "Mesh.h"
#include "Transform.h"

#include "glm/glm.hpp"

#include <string>

namespace Common
{
	class GameObject;
	struct Contact;

	class Collider
	{
	public:
		Collider(GameObject * gameObject, CollisionBody * body = 0) : m_gameObject(gameObject), m_collisionBody(body) {}
		virtual ~Collider() {}

		std::string name;

		inline CollisionBody * collisionBody() const { return m_collisionBody; }
		virtual bool isInside(const glm::vec3 & /*point*/, Contact * /*contact*/) const { return false; }

		Transform transform() const;

	private:
		GameObject * m_gameObject;
		CollisionBody * m_collisionBody;
	};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider(GameObject * gameObject, CollisionBody * body = 0) 
			: Collider(gameObject, body),
			m_position(glm::vec3(0.0f)),
			m_radius(1.0f) {}

		virtual ~SphereCollider() {}

		virtual bool isInside(const glm::vec3 & point, Contact * contact) const;

		glm::vec3 m_position;
		glm::float_t m_radius;
	};

	class PlaneCollider : public Collider
	{
	public:
		PlaneCollider(GameObject * gameObject, CollisionBody * body = 0) : Collider(gameObject, body) {}
		virtual ~PlaneCollider() {}

		glm::vec3 m_normal;
		glm::float_t m_d;
	};

	class MeshCollider : public Collider
	{
	public:
		MeshCollider(GameObject * gameObject, CollisionBody * body = 0, bool usePoints = false) : Collider(gameObject, body), m_usePoints(usePoints) {}
		virtual ~MeshCollider() {}

		virtual bool isInside(const glm::vec3 & point, Contact * contact) const;

		Mesh * m_mesh;
		BVH * m_bvh;
		bool m_usePoints;
	};
}

#endif /* COMMON_COLLIDER */
