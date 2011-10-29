#ifndef RIGIDBODY_H
#define RIGIDBODY_H

namespace Common
{
	class GameObject;

	class RigidBody
	{
	public:
		RigidBody(GameObject * gameObject);
		virtual ~RigidBody();

		virtual void update(float dt);

		void addForce(glm::vec3 force);
		void addTorque(glm::vec3 torque);

		inline Transform transform()
		{
			return Transform::Translation(m_position);
		}

	private:
		GameObject * m_gameObject;

		glm::vec3 m_position;
		glm::mat3 m_rotation;

		glm::float_t m_mass;
		glm::mat3 m_inertiaTensor;

		glm::vec3 m_linearMomentum; // m*v
		glm::vec3 m_angularMomentum; //I*w
	};
}
#endif // RIGIDBODY_H
