/**
 * Copyright 2011-2012 Sebastian Eriksson.
 * This file is part of Motion.
 *
 * Motion is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Motion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Motion. If not, see <http://www.gnu.org/licenses/>.
 */

#include "CollisionDetector.h"
#include "Physics.h"
#include "Trace.h"
#include "Utils.h"

namespace Common
{
	Physics::Physics()
	{
		m_lineRenderer = new LineRenderer(0);
	}

	Physics::~Physics()
	{
		delete m_lineRenderer;
	}

	void Physics::addObject(RigidBody * rigidBody)
	{
		assert(rigidBody);

		m_objects.push_back(rigidBody);
	}

	void Physics::removeObject(RigidBody * rigidBody)
	{
		// TODO: should be able to remove objects
		(void)rigidBody;
	}

	void Physics::step(float dt)
	{
		// delta time in seconds
		float elapsed = dt / 1000.0f;

		for (ObjectIterator it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			RigidBody * obj = *it;

			// 1. Apply forces and torques
			// TODO: crete more generic force generators
			obj->applyForce(glm::vec3(0.0f, -9.81f, 0.0f) * obj->m_mass);

			// 2. Update rigid-body attributes (numerical integration)
			obj->integrate(elapsed);
		}

		// 3. Contact generator (generate collision data for next step)
		std::vector<CollisionData> collisions;
		CollisionDetector::instance()->detectCollisions(collisions);

		// 4. Resolve collisions and write velocity to rigidbody data
		for (unsigned i = 0; i < collisions.size(); i++)
		{
			const CollisionData & data = collisions[i];

			// TODO: make Physics support soft bodies as well
			//assert(data.type == CollisionData::RIGID);

			resolveVelocity(data);
			resolveInterpenetration(data);
		}
	}

	void Physics::resolveVelocity(const CollisionData & data)
	{
		RigidBody * body1 = static_cast<RigidBody*>(data.bodies[0]);
		RigidBody * body2 = static_cast<RigidBody*>(data.bodies[1]);

		Contact * contact = data.contacts[0];

		glm::vec3 relativeVelocity = body1->velocity() + 
			glm::cross(body1->m_angularVelocity, contact->point - body1->m_position);

		if (body2) {
			relativeVelocity -= body2->velocity() +
				glm::cross(body2->m_angularVelocity, contact->point - body2->m_position);
		}

		float separation = glm::dot(relativeVelocity, contact->normal);

		// check if the bodies are already moving apart
		if (separation > 0)
			return;

		// calculate linear momentum
		float denom = 1.0f / body1->m_mass;
		if (body2) denom += 1.0f / body2->m_mass;

		// calculate angular momentum

		glm::vec3 r1 = contact->point - body1->m_position;

		glm::mat3 inertiaTensor = body1->m_rotation * body1->m_inertiaTensor * glm::transpose(body1->m_rotation);
		denom += glm::dot(glm::cross(glm::inverse(inertiaTensor) * glm::cross(r1, contact->normal), r1), contact->normal);

		if (body2) {
			glm::vec3 r2 = contact->point - body2->m_position;

			glm::mat3 inertiaTensor = body2->m_rotation * body2->m_inertiaTensor * glm::transpose(body2->m_rotation);
			denom += glm::dot(glm::cross(glm::inverse(inertiaTensor) * glm::cross(r2, contact->normal), r2), contact->normal);
		}

		// calculate impulse magnitude
		float impulse = -separation * (1 + data.restitution) / denom;

		// calculate tangential velocity
		glm::vec3 tangent(0.0f);
		float tangentVelocity = 0.0f;

		if (separation != 0)
		{
			glm::vec3 VonN = relativeVelocity - glm::dot(relativeVelocity, contact->normal) * contact->normal;
			tangentVelocity = glm::length(VonN);
			if (tangentVelocity > 0)
				tangent = glm::normalize(VonN);
		}

		// calculate impulse based on tangential friction
		if (tangentVelocity != 0)
		{
			// calculate linear momentum
			denom = 1.0f / body1->m_mass;
			if (body2) denom += 1.0f / body2->m_mass;

			glm::mat3 inertiaTensor = body1->m_rotation * body1->m_inertiaTensor * glm::transpose(body1->m_rotation);
			denom += glm::dot(glm::cross(glm::inverse(inertiaTensor) * glm::cross(r1, tangent), r1), tangent);

			if (body2) {
				glm::vec3 r2 = contact->point - body2->m_position;

				glm::mat3 inertiaTensor = body2->m_rotation * body2->m_inertiaTensor * glm::transpose(body2->m_rotation);
				denom += glm::dot(glm::cross(glm::inverse(inertiaTensor) * glm::cross(r2, tangent), r2), tangent);
			}
		
			// desired impulse to overcome tangent velocity
			float desiredImpulse = tangentVelocity / denom;

			// check if impulse is within the friction cone
			if (desiredImpulse > data.friction * impulse)
				tangent *= impulse * data.friction;
			else
				tangent *= desiredImpulse;
		}

		// apply impulses
		body1->applyImpulse(impulse * contact->normal - tangent, contact->point);
		if (body2) body2->applyImpulse(-(impulse * contact->normal - tangent), contact->point);
	}

	void Physics::resolveInterpenetration(const CollisionData & data)
	{
		RigidBody * body1 = static_cast<RigidBody*>(data.bodies[0]);
		RigidBody * body2 = static_cast<RigidBody*>(data.bodies[1]);

		Contact * contact = data.contacts[0];

		float invMass = 0.0f;
		if (body1) invMass += 1.0f / body1->m_mass;
		if (body2) invMass += 1.0f / body2->m_mass;

		glm::vec3 movePerIMass = contact->normal * (contact->penetration / invMass);

		if (body1) body1->m_position += movePerIMass * (1.0f / body1->m_mass);
		else if (body2) body2->m_position -= movePerIMass * (1.0f / body2->m_mass);
	}

	void Physics::explode(const glm::vec3 & center)
	{
		for (ObjectIterator it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			RigidBody * obj = *it;

			glm::vec3 cp = obj->position() - center;

			// TODO: don't hardcode this
			obj->applyForce(cp * (30.0f / glm::length(cp)));
		}
	}

	float Physics::calculateEnergy(float potentialZero)
	{
		float U_total = 0;
		float Ek_total = 0;
		float Erot_total = 0;

		for (ObjectIterator it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			RigidBody * obj = *it;

			float U = obj->m_mass * 9.81f * (obj->position().y - potentialZero);

			float v = glm::length(obj->velocity());
			float Ek = 0.5f * (obj->m_mass * v * v);
			
			float w = glm::length(obj->m_angularVelocity);
			float Erot = 0.5f * (obj->m_inertiaTensor[0][0] * w * w);

			U_total += U;
			Ek_total += Ek;
			Erot_total += Erot;
		}

		return U_total + Ek_total + Erot_total;
	}

	void Physics::visualize()
	{
		m_lineRenderer->clear();

		for (ObjectIterator it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			RigidBody * body = *it;

			LineRenderer::vertex a;
			a.position = body->m_position;
			a.color = glm::vec4(1.0f);

			LineRenderer::vertex b;
			b.position = body->m_position + body->m_velocity;
			b.color = glm::vec4(1.0f);

			m_lineRenderer->addLine(a, b);
		}

		m_lineRenderer->draw();
	}
}
