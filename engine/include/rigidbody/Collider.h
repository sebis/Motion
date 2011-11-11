#ifndef COMMON_COLLIDER
#define COMMON_COLLIDER

#include "RigidBody.h"

#include "glm/glm.hpp"

#include <string>

namespace Common
{
	class GameObject;

	class Collider
	{
	public:
		Collider(GameObject * gameObject, RigidBody * rigidBody = 0) : m_gameObject(gameObject), m_rigidBody(rigidBody) {}
		virtual ~Collider() {}

		std::string name;

		inline RigidBody * rigidBody() const { return m_rigidBody; }

		Transform transform();

	private:
		GameObject * m_gameObject;
		RigidBody * m_rigidBody;
	};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider(GameObject * gameObject, RigidBody * rigidBody = 0) 
			: Collider(gameObject, rigidBody),
			m_position(glm::vec3(0.0f)),
			m_radius(1.0f) {}

		virtual ~SphereCollider() {}

		glm::vec3 m_position;
		glm::float_t m_radius;
	};

	class PlaneCollider : public Collider
	{
	public:
		PlaneCollider(GameObject * gameObject, RigidBody * rigidBody = 0) : Collider(gameObject, rigidBody) {}
		virtual ~PlaneCollider() {}

		glm::vec3 m_normal;
		glm::float_t m_d;
	};
}

#endif /* COMMON_COLLIDER */
