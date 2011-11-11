#include "CollisionDetector.h"
#include "Physics.h"
#include "Trace.h"
#include "Utils.h"

#include <GL\glew.h>

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

		//calculateEnergy();
	}

	void Physics::resolveVelocity(const CollisionData & data, float elapsed)
	{
		float restitution = 0.8f;
		const float s_mu = 0.10f;
		const float d_mu = 0.05f;

		RigidBody * body1 = data.bodies[0];
		RigidBody * body2 = data.bodies[1];

		if (!body1) {
			body1 = body2;
			body2 = 0;
		}
		assert(body1);

		glm::vec3 relativeVelocity = body1->velocity() + 
			glm::cross(body1->m_angularVelocity, data.point - body1->m_position);

		if (body2) {
			relativeVelocity -= body2->velocity() +
				glm::cross(body2->m_angularVelocity, data.point - body2->m_position);
		}

		/*float velocityFromAcc = glm::dot(body1->m_acceleration * elapsed, data.normal);
		if (body2) velocityFromAcc -= glm::dot(body2->m_acceleration * elapsed, data.normal);

		relativeVelocity += velocityFromAcc;*/

		float separation = glm::dot(relativeVelocity, data.normal);

		// check if the bodies are already moving apart
		if (separation > 0)
			return;

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

		float impulse = -separation * (1 + restitution) / invMass;
		//float impulse = (-separation - restitution * (separation - velocityFromAcc)) / invMass;

		// project relativeVelocity onto plane defined by data.normal and data.point
		/*glm::vec3 tangent(0.0f);
		if (separation < 0) {
			glm::vec3 temp = glm::cross(data.normal, relativeVelocity);
			temp = glm::cross(temp, data.normal);
			float length = glm::length(temp);
			if (length > 0.0001f) {
				tangent = glm::normalize(temp);
			}
		}*/

		glm::vec3 tangent(0.0f);
		if (separation != 0)
		{
			glm::vec3 VonN = relativeVelocity - glm::dot(relativeVelocity, data.normal) * data.normal;
			if (glm::length(VonN) > 0)
				tangent = glm::normalize(VonN);
		}

		float js = s_mu * impulse;
		float jd = d_mu * impulse;

		/*float tSeparation = glm::dot(relativeVelocity, tangent);

		if (tSeparation == 0 && body1->m_mass * tSeparation <= js)
			body1->applyImpulse(impulse * data.normal - body1->m_mass * tSeparation * tangent, data.point);
		else
			body1->applyImpulse(impulse * data.normal - jd * tangent, data.point);

		if (body2)
		{
			if (tSeparation == 0 && body2->m_mass * tSeparation <= js)
				body2->applyImpulse(impulse * data.normal - body2->m_mass * tSeparation * tangent, data.point);
			else
				body2->applyImpulse(-impulse * data.normal - jd * tangent, data.point);
		}*/

		body1->applyImpulse(impulse * data.normal - jd * tangent, data.point);
		if (body2) body2->applyImpulse(-impulse * data.normal - jd * tangent, data.point);

		/*body1->applyImpulse(impulse * data.normal, data.point);
		if (body2) body2->applyImpulse(-impulse * data.normal, data.point);*/
	}

	void Physics::resolveInterpenetration(const CollisionData & data)
	{
		RigidBody * body1 = data.bodies[0];
		RigidBody * body2 = data.bodies[1];

		Trace::info("Resolving interpenetration for %s\n", data.result.c_str());

		if (glm::abs(data.penetration) > 0.1f)
		{
			int dummy = 0;
		}

		float totalInverseMass = 0.0f;
		if (body1) totalInverseMass = 1.0f / body1->m_mass;
		if (body2) totalInverseMass += 1.0f / body2->m_mass;

		if (totalInverseMass <= 0)
			return;

		glm::vec3 movePerIMass = data.normal * (data.penetration / totalInverseMass);
		//Trace::info("movePerIMass: %f %f %f\n", movePerIMass.x, movePerIMass.y, movePerIMass.z);
		if (body1) {
			body1->m_position += movePerIMass * (1.0f / body1->m_mass);
			Utils::print_vector("move body1", movePerIMass * (1.0f / body1->m_mass));
		}
		else if (body2) {
			body2->m_position -= movePerIMass * (1.0f / body2->m_mass);
			Utils::print_vector("move body2", movePerIMass * (1.0f / body2->m_mass));
		}
	}

	void Physics::explode(const glm::vec3 & center)
	{
		for (ObjectIterator it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			RigidBody * obj = *it;

			glm::vec3 cp = obj->position() - center;

			obj->applyImpulse(cp * (100.0f / glm::length(cp)), obj->position());
		}
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

	void Physics::visualize()
	{
		for (ObjectIterator it = m_objects.begin(); it != m_objects.end(); ++it)
		{
		}
	}
}
