#ifndef COMMON_COLLISIONDETECTOR
#define COMMON_COLLISIONDETECTOR

#include "Collider.h"
#include "RigidBody.h"

#include <vector>
#include <string>

namespace Common
{
	namespace
	{
		const float COF_BALL_BALL = 0.95f;
		const float COF_BALL_RAIL = 0.7f;
		const float COF_BALL_TABLE = COF_BALL_RAIL;
	
		const float FRICTION_BALL_BALL = 0.05f;
		const float FRICTION_BALL_CLOTH = 0.5f;
	}

	struct CollisionData
	{
		glm::vec3 point;
		glm::vec3 normal;
		float penetration;
		float restitution;
		std::string result;

		RigidBody * bodies[2];
	};

	class CollisionDetector
	{
	public:

		static CollisionDetector * instance();
		~CollisionDetector() {}

		static bool SphereAndSphere(SphereCollider * a, SphereCollider * b, CollisionData * data);
		static bool SphereAndPlane(SphereCollider * a, PlaneCollider * b, CollisionData * data);

		inline void addCollider(Collider * collider) { m_colliders.push_back(collider); }
		inline void clear() { m_colliders.clear(); }

		void detectCollisions(std::vector<CollisionData> & collisions);

	private:
		CollisionDetector() {}

		typedef std::vector<Collider*> Colliders;
		Colliders m_colliders;

		static CollisionDetector * s_instance;
	};
}

#endif /* COMMON_COLLISIONDETECTOR */