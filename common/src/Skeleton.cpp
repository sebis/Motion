#include "Skeleton.h"

namespace Common
{
	void Skeleton::update(float dt)
	{
		GameObject::update(dt);

		computeIK();
	}

	void Skeleton::computeIK()
	{
		// Check every IK chain separately
		for (int i = 0; i < m_endEffectors.size(); i++)
		{
			// end effector we want to reach
			Bone * goal = m_endEffectors[i];

			// current joint, in the beginning last bone in the IK chain
			Bone * joint = goal->parent();

			// This will always point to the last bone in the IK chain
			Bone * tip = goal->parent();

			glm::vec3 targetPosition = glm::vec3(goal->transform().world()[3]);

			// Loop through the bone hierarchy from tip to the root bone
			// TODO: loop iteratively
			while (joint != 0)
			{
				glm::vec3 jointPosition = glm::vec3(joint->transform().world()[3]);

				// Get tip location by using the local transform
				glm::vec3 tipPosition = glm::vec3((tip->transform().world() * tip->localTransform().world())[3]);

				//float distance = glm::distance(targetPosition, tipPosition);

				// Loop through all possible axes (degress of freedom)
				for (int k = 0; k < 3; k++)
				{
					// Get the current axis
					RotationAxis * rotAxis = joint->axis(k);
					glm::vec3 axis = rotAxis->m_axis;

					// Check if this degree of freedom is enabled for this joint
					if (!rotAxis->free())
						continue;

					glm::vec3 toTip = tipPosition - jointPosition;
					glm::vec3 toTarget = targetPosition - tipPosition;

					// get gradient describing the direction of the simulated annealing
					glm::vec3 movement_vector = glm::cross(toTip, axis);
					float gradient = -glm::dot(movement_vector, toTarget);

					// Check if the rotation is constrained
					float newAngle = joint->transform().rotation()[k] + gradient;
					if (rotAxis->valid(newAngle))
						joint->transform().rotate(gradient * axis);
				}

				// Get next joint in chain
				joint = joint->parent();
			}
		}
	}
}