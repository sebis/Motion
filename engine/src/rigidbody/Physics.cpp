#include "CollisionDetector.h"
#include "Physics.h"
#include "Trace.h"

namespace Common
{
	Physics::Physics()
	{
	}

	Physics::~Physics()
	{
	}

	void Physics::addObject(RigidBody * rigidBody)
	{
		assert(rigidBody);

		m_objects.push_back(rigidBody);
	}

	void Physics::removeObject(RigidBody * rigidBody)
	{
		// TODO: should be able to remove objects
	}

	void Physics::step(float dt)
	{
		// delta time in seconds
		float elapsed = dt / 1000.0f;

		for (ObjectIterator it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			RigidBody * obj = *it;
			//obj->update(dt);

			// 1. Apply forces and torques (write to rigidbody data)
			obj->applyForce(glm::vec3(0.0f, -9.81f, 0.0f) * obj->m_mass);

			// 2. Update rigid-body attributes (numerical integration), (write integrated data to rigidbody)
			obj->integrate(elapsed);
		}

		// 3. Contact generator (generate collision data for next step)
		std::vector<CollisionData> collisions;
		CollisionDetector::instance()->detectCollisions(collisions);

		// 4. Write postcollision position and velocity to rigidbody data
		for (unsigned i = 0; i < collisions.size(); i++)
		{
			const CollisionData & data = collisions[i];

			resolveVelocity(data, elapsed);
			resolveInterpenetration(data);
		}

		calculateEnergy();
	}

	void Physics::resolveVelocity(const CollisionData & data, float elapsed)
	{
		RigidBody * body1 = data.bodies[0];
		RigidBody * body2 = data.bodies[1];

		glm::vec3 relativeVelocity(0.0f);

		if (body1) relativeVelocity = body1->velocity();
		if (body2) relativeVelocity -= body2->velocity();

		float separation = glm::dot(relativeVelocity, data.normal);

		if (separation > 0)
			return;

		const float restitution = 1.0f;
		float newSepVelocity = -separation * restitution;

		glm::vec3 accCausedVelocity(0.0f);
		if (body1) accCausedVelocity = body1->m_acceleration;
		if (body2) accCausedVelocity -= body2->m_acceleration;

		float accCausedSepVelocity = glm::dot(accCausedVelocity, data.normal) * elapsed;

		if (accCausedSepVelocity < 0)
		{
			newSepVelocity += restitution * accCausedSepVelocity;
			newSepVelocity = glm::max(newSepVelocity, 0.0f);
		}

		float deltaVelocity = newSepVelocity - separation;

		float totalInverseMass = 0.0f;
		if (body1) totalInverseMass = 1.0f / body1->m_mass;
		if (body2) totalInverseMass += 1.0f / body2->m_mass;

		if (totalInverseMass <= 0)
			return;

		float impulse = deltaVelocity / totalInverseMass;

		glm::vec3 impulsePerIMass = data.normal * impulse;
		if (body1) body1->applyImpulse(impulsePerIMass * (1.0f / body1->m_mass), data.point);
		if (body2) body2->applyImpulse(-impulsePerIMass * (1.0f / body2->m_mass), data.point);
	}

	void Physics::resolveInterpenetration(const CollisionData & data)
	{
		if (data.penetration <= 0)
				return;

		RigidBody * body1 = data.bodies[0];
		RigidBody * body2 = data.bodies[1];

		float totalInverseMass = 0.0f;
		if (body1) totalInverseMass = 1.0f / body1->m_mass;
		if (body2) totalInverseMass += 1.0f / body2->m_mass;

		if (totalInverseMass <= 0)
			return;

		glm::vec3 movePerIMass = data.normal * (data.penetration / totalInverseMass);
		//Trace::info("movePerIMass: %f %f %f\n", movePerIMass.x, movePerIMass.y, movePerIMass.z);
		if (body1) body1->m_position += movePerIMass * (1.0f / body1->m_mass);
		if (body2) body2->m_position += movePerIMass * (1.0f / body2->m_mass);
	}

	void Physics::calculateEnergy()
	{
		float U_total = 0;
		float Ek_total = 0;
		for (ObjectIterator it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			RigidBody * obj = *it;

			// choose y-coordinate 0 as the zero-potential level
			float U = obj->m_mass * 9.81f * obj->position().y;

			float v = glm::length(obj->velocity());
			float Ek = 0.5f * (obj->m_mass * v * v);

			U_total += U;
			Ek_total += Ek;
		}

		static float total = 0;
		if (total < U_total + Ek_total) {
			Trace::warning("INSTABILITY");
			total = U_total + Ek_total;
		}

		Trace::info("System energy: %.2f + %.2f = %.2f\n", U_total, Ek_total, U_total + Ek_total);
	}
}
