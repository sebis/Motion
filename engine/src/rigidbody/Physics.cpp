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
			//resolveInterpenetration(data);
		}

		//calculateEnergy();
	}

	void Physics::resolveVelocity(const CollisionData & data, float elapsed)
	{
		RigidBody * body1 = data.bodies[0];
		RigidBody * body2 = data.bodies[1];

		if (!body1) {
			body1 = body2;
			body2 = 0;
		}
		assert(body1);

		glm::vec3 relativeVelocity = body1->velocity() + 
			glm::cross(body1->m_angularVelocity, data.point - body1->m_position);

		Trace::info("data-point: %f %f %f %f\n", data.point.x, data.point.y, data.point.z, data.penetration);
		Trace::info("length: %f\n", glm::length(data.point - body1->m_position));

		if (body2) {
			relativeVelocity -= body2->velocity() +
				glm::cross(body2->m_angularVelocity, data.point - body2->m_position);
		}

		float separation = glm::dot(relativeVelocity, data.normal);

		// check if the bodies are already moving apart
		if (separation > 0)
			return;

		const float restitution = 1.0f;

		// Should avoid jumping with microcollisions
		/*glm::vec3 accCausedVelocity(0.0f);
		if (body1) accCausedVelocity = body1->m_acceleration;
		if (body2) accCausedVelocity -= body2->m_acceleration;

		float accCausedSepVelocity = glm::dot(accCausedVelocity, data.normal) * elapsed;

		if (accCausedSepVelocity < 0)
		{
			newSepVelocity += restitution * accCausedSepVelocity;
			newSepVelocity = glm::max(newSepVelocity, 0.0f);
		}*/

		//float deltaVelocity = newSepVelocity - separation;
		//float deltaVelocity = -(1 + restitution) * separation;

		float invMass = 1.0f / body1->m_mass;
		if (body2) invMass += 1.0f / body2->m_mass;

		if (invMass <= 0)
			return;

		glm::vec3 r1 = data.point - body1->m_position;

		glm::mat3 inertiaTensor = body1->m_rotation * body1->m_inertiaTensor * glm::transpose(body1->m_rotation);
		glm::vec3 haka1 = glm::inverse(inertiaTensor) * glm::cross(r1, data.normal);

		invMass += glm::dot(glm::cross(haka1, r1), data.normal);

		if (body2) {
			glm::vec3 r2 = data.point - body2->m_position;

			glm::mat3 inertiaTensor = body2->m_rotation * body2->m_inertiaTensor * glm::transpose(body2->m_rotation);
			glm::vec3 haka2 = glm::inverse(inertiaTensor) * glm::cross(r2, data.normal);

			invMass += glm::dot(glm::cross(haka2, r2), data.normal);
		}

		/*glm::vec3 deltaVelWorld = glm::cross(data.point - body1->m_position, data.normal);
		glm::mat3 inertiaTensor = body1->m_rotation * body1->m_inertiaTensor * glm::transpose(body1->m_rotation);
		deltaVelWorld = glm::inverse(inertiaTensor) * deltaVelWorld;
		deltaVelWorld = glm::cross(deltaVelWorld, data.point - body1->m_position);

		float deltaVelocity = glm::dot(deltaVelWorld, data.normal);
		deltaVelocity += 1 / body1->m_mass;

		if (body2) {
			glm::vec3 deltaVelWorld = glm::cross(data.point - body2->m_position, data.normal);
			glm::mat3 inertiaTensor = body2->m_rotation * body2->m_inertiaTensor * glm::transpose(body2->m_rotation);
			deltaVelWorld = glm::inverse(inertiaTensor) * deltaVelWorld;
			deltaVelWorld = glm::cross(deltaVelWorld, data.point - body2->m_position);

			deltaVelocity += glm::dot(deltaVelWorld, data.normal);
			deltaVelocity += 1 / body1->m_mass;
		}

		float impulse = desiredDeltaVelocity / deltaVelocity;*/
		
		//float impulse = deltaVelocity / invMass;

		float impulse = -separation*(1.0f + 1) / invMass;

		// project relativeVelocity onto plane defined by data.normal and data.point
		glm::vec3 tangent(0.0f);
		if (separation < 0) {
			glm::vec3 temp = glm::cross(data.normal, relativeVelocity);
			temp = glm::cross(temp, data.normal);
			float length = glm::length(temp);
			if (length > 0.0001f) {
				tangent = glm::normalize(temp);
			}
		}

		/*glm::vec3 tangent(0.0f);
		if (glm::dot(relativeVelocity, data.normal) != 0)
		{
			tangent = glm::normalize(relativeVelocity - glm::dot(relativeVelocity, data.normal) * data.normal);
		}*/

		const float friction = 0.1f;

		//glm::vec3 impulsePerIMass = data.normal * impulse;
		//Trace::info("normal: %f %f %f\n", data.normal.x, data.normal.y, data.normal.z);
		//Trace::info("tangent: %f %f %f\n", tangent.x, tangent.y, tangent.z);

		glm::vec3 frictionImpulse(0.0f);
		if (glm::abs(glm::dot(relativeVelocity, tangent)) > 0.0f) {
			glm::vec3 frictionImpulse = friction * impulse * tangent;
			//Trace::info("%f vs %f\n", glm::length(impulse * data.normal), glm::length(frictionImpulse));
		}

		//tangent = impulse * data.normal - impulse;

		if (body1) body1->applyImpulse(impulse * data.normal - friction * impulse * tangent, data.point);
		if (body2) body2->applyImpulse(-impulse * data.normal - friction * impulse * tangent, data.point);

		/*float invMass = 0.0f;
		if (body1) invMass += 1.0f/body1->m_mass;
		if (body2) invMass += 1.0f/body2->m_mass;

		float impulse = (-(1+1.0f) * glm::dot(relativeVelocity, data.normal)) / (glm::dot(data.normal, data.normal) * invMass);

		//float impulse = (-(1+1.0f) * glm::dot(relativeVelocity, data.normal)) / invMass;

		glm::vec3 tangent = glm::cross(data.normal, glm::cross(data.normal, glm::normalize(relativeVelocity)));

		float friction = 0.5f;
		glm::vec3 resistance = friction * impulse * tangent;
		Trace::info("resistance: %f %f %f\n", tangent.x, tangent.y, tangent.z);

		if (body1) body1->applyImpulse((impulse * data.normal + friction * impulse * tangent) / body1->m_mass, data.point);
		if (body2) body2->applyImpulse(-(impulse * data.normal + friction * impulse * tangent) / body2->m_mass, data.point);*/
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
		float Erot_total = 0;

		for (ObjectIterator it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			RigidBody * obj = *it;

			// choose y-coordinate 0 as the zero-potential level
			float U = obj->m_mass * 9.81f * obj->position().y;

			float v = glm::length(obj->velocity());
			float Ek = 0.5f * (obj->m_mass * v * v);
			
			float w = glm::length(obj->m_angularVelocity);
			float Erot = 0.5f * (obj->m_inertiaTensor[0][0] * w * w);

			U_total += U;
			Ek_total += Ek;
			Erot_total += Erot;
		}

		static float total = 0;
		if (total < U_total + Ek_total + Erot_total) {
			Trace::warning("INSTABILITY");
			total = U_total + Ek_total + Erot_total;
		}

		Trace::info("System energy: %.2f + %.2f + %.2f = %.2f\n", U_total, Ek_total, Erot_total, total);
	}
}
