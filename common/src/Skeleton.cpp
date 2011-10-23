#include "Skeleton.h"

namespace Common
{
	void Skeleton::update(float dt)
	{
		GameObject::update(dt);

		computeIK();
	}

	/*void Skeleton::computeIK()
	{
		for (int i = 0; i < m_endEffectors.size(); i++)
		{
			Bone * goal = m_endEffectors[i];
			Bone * joint = goal->parent();
			Bone * origin = joint->parent();

			// transform goal and origin locations to world space
			glm::vec3 offset = glm::vec3(goal->transform().world()[3]) - glm::vec3(origin->transform().world()[3]);
			float x = offset.x;
			float y = -offset.y;

			// TODO: get bone lengths
			float L1 = 5.0f;//glm::distance(origin->transform().position(), joint->transform().position());
			float L2 = 5.0f;//glm::distance(joint->transform().position(), goal->transform().position());

			//Trace::info("L: %f %f\n", x, y);

			float o2 = std::acos((x*x + y*y - L1*L1 - L2*L2)/(2*L1*L2));
			float n1 = (-x*(L2*std::sin(o2)) + y*(L1 + L2*std::cos(o2)));
			float n2 = (y*(L2*std::sin(o2)) + x*(L1 + L2*std::cos(o2)));
			float o1 = std::atan(n1/n2);

			// TODO: 90 degress offset? why?
			if (std::abs(o1) >= 0.0001f)
				origin->transform().rotation() = glm::vec3(0.0f, 0.0f, 90.0f + (-o1 / 3.14159265 * 180.0));
			if (std::abs(o2) >= 0.0001f)
				joint->transform().rotation() = glm::vec3(0.0f, 0.0f, -o2 / 3.14159265 * 180.0);
		}
	}*/

	void Skeleton::computeIK()
	{
		for (int i = 0; i < m_endEffectors.size(); i++)
		{
			Bone * goal = m_endEffectors[i];
			Bone * joint = goal->parent();
			Bone * tip = goal->parent();

			glm::vec3 targetPosition = glm::vec3(goal->transform().world()[3]);

			while (joint != 0)
			{
				glm::vec3 jointPosition = glm::vec3(joint->transform().world()[3]);
				glm::vec3 tipPosition = glm::vec3((tip->transform().world() * tip->m_localTransform.world())[3]);

				float distance = glm::distance(targetPosition, tipPosition);

				//Trace::info("target: %f %f %f\n", targetPosition.x, targetPosition.y, targetPosition.z);
				//Trace::info("joint: %f %f %f\n", jointPosition.x, jointPosition.y, jointPosition.z);
				//Trace::info("tip: %f %f %f\n", tipPosition.x, tipPosition.y, tipPosition.z);
				Trace::info("distance to target: %f\n", distance);

				// TODO: more than one DOF?
				glm::vec3 axis = joint->axis();

				glm::vec3 ToTip = tipPosition - jointPosition;
				glm::vec3 ToTarget = targetPosition - tipPosition;

				glm::vec3 movement_vector = glm::cross(ToTip, -(joint->axis()));
				float gradient = glm::dot(movement_vector, ToTarget);

				if (joint->name() == "RightKnee") {
					//Trace::info("Wanting to rotate: %f, allowed: %f\n", gradient, glm::clamp(gradient, -180.0f, 0.0f));
					float newz = joint->transform().rotation().z + gradient;
					if (newz >= -180.0f && newz < 0.0f) 
						joint->transform().rotate(gradient * axis);
				} else {
					joint->transform().rotate(gradient * axis);
				}

				if (joint->name() == "RightKnee") {
					//Trace::info("rotating: %f around +Z (%f %f)\n", joint->transform().rotation().z, min, max);
				}

				joint = joint->parent();
			}
		}
	}
}