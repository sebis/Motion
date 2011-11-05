#ifndef COMMON_PHYSICS
#define COMMON_PHYSICS

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

	private:
		void resolveVelocity(const CollisionData & data, float dt);
		void resolveInterpenetration(const CollisionData & data);

		void calculateEnergy();

		typedef std::vector<RigidBody *> Objects;
		typedef Objects::iterator ObjectIterator;

		Objects m_objects;
	};
}

#endif /* COMMON_PHYSICS */