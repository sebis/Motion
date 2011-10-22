#include "GameObject.h"
#include "KeyframeAnimator.h"
#include "Transform.h"
#include "Trace.h"

#include "glm/glm.hpp"

namespace Interpolation
{
	template<>
	void KeyframeAnimator<glm::vec3>::orient(glm::vec3 tangent)
	{
		if (m_orient) {
			glm::vec3 up = glm::vec3(0, 1, 0);

			glm::vec3 forward = glm::normalize(tangent);
			glm::vec3 right = glm::cross(up, forward);

			up = glm::cross(forward, right);

			glm::mat3 r(right, up, forward);

			// Extract euler rotations from the rotation matrix
			/* This is basically a useless operation because we could just use the computed
			   rotation matrix for the transformation. However, The current transform component
			   works with euler angles so that they can be interpolated. Thus we extract and save
			   the rotation as euler angles instead. */
			{
				float r00 = r[0][0], r01 = r[1][0], r02 = r[2][0],
					  r10 = r[0][1], r11 = r[1][1], r12 = r[2][1],
					  r20 = r[0][2], r21 = r[1][2], r22 = r[2][2];

				float y = asin(r02);
				float x = atan2(-r12,r22);
				float z = atan2(-r01,r00);

				m_gameObject->transform().rotation() = glm::vec3(x, y, z) * float(180.0/3.14159265);
			}
		}
	}
};
