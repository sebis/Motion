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
		for (unsigned i = 0; i < m_endEffectors.size(); i++)
		{
			// end effector we want to reach
			Bone * goal = m_endEffectors[i];

			// current joint, in the beginning last bone in the IK chain
			Bone * joint = goal->parent();

			// This will always point to the last bone in the IK chain
			Bone * tip = goal->parent();

			glm::vec3 targetPosition = glm::vec3(goal->transform().world()[3]);

			// TODO: better heuristic for terminating the iteration
			for (int j = 0; j < 3; j++)
			{
				// Loop through the bone hierarchy from tip to the root bone
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

				// Reset back to tip
				joint = goal->parent();
			}
		}
	}
}