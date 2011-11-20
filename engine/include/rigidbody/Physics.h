#ifndef COMMON_PHYSICS
#define COMMON_PHYSICS

#include "LineRenderer.h"
#include "RigidBody.h"

#include <vector>

namespace Common
{
	struct CollisionData;

	class Physics
	{
	public:
		Physics();
		~Physics();

		void addObject(RigidBody * rigidBody);
		void removeObject(RigidBody * rigidBody);

		void step(float dt);
		void visualize();

		void explode(const glm::vec3 & center);

		float calculateEnergy(float potentialZero = 0.0f);

	private:
		void resolveVelocity(const CollisionData & data);
		void resolveInterpenetration(const CollisionData & data);

		typedef std::vector<RigidBody *> Objects;
		typedef Objects::iterator ObjectIterator;

		Objects m_objects;

		LineRenderer * m_lineRenderer;
	};
}

#endif /* COMMON_PHYSICS */
