#include "GameObject.h"
#include "RigidBody.h"

namespace Common
{
	RigidBody::RigidBody(GameObject * gameObject)
		: m_gameObject(gameObject),
		  m_position(glm::vec3(0.0f)),
		  m_rotation(glm::mat3(1.0f)),
		  m_linearMomentum(glm::vec3(0.0f)),
		  m_angularMomentum(glm::vec3(0.0f))

	{
		m_position = glm::vec3(0.0f, 10.0f, 0.0f);
		// TODO: calculate these from the object we want to simulate
		m_mass = 10.0f;

		// Inertia tensor: This could be set manually, or if not specified
		// should be calculated by all collider objects and their mass centers

		float radius = 1.0f;
		// sphere cnetered at origin
		m_inertiaTensor = glm::mat3(2.0f/5.0f * m_mass * radius * radius);
	}

	RigidBody::~RigidBody()
	{
	}

	void RigidBody::update(float dt)
	{
		// delta time in seconds
		float elapsed = dt / 1000.0f;

		//glm::mat3 inertiaTensor = m_rotation * m_inertiaTensor * glm::transpose(m_rotation);

		//glm::vec3 angularVelocity = (glm::inverse(inertiaTensor) * m_angularMomentum);
		glm::vec3 velocity = (m_linearMomentum / m_mass);

		m_position += velocity * elapsed;
		//m_rotation += glm::mat3(glm::cross(angularVelocity, m_rotation[0]), glm::cross(angularVelocity, m_rotation[1]), glm::cross(angularVelocity, m_rotation[2])) * dt;

		m_linearMomentum += glm::vec3(0.0f, -9.81, 0.0f) * m_mass * elapsed;
		//m_angularMomentum += torques * dt;

		Trace::info("position: %f %f %f\n", m_position.x, m_position.y, m_position.z);
		Trace::info("linearMomentum: %f %f %f\n", m_linearMomentum.x, m_linearMomentum.y, m_linearMomentum.z);
	}
}
