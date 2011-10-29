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
		if (m_flags & ORIENT) {
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
				float y = asin(r[2][0]);
				float x = atan2(-r[2][1],r[2][2]);
				float z = atan2(-r[1][0],r[0][0]);

				m_gameObject->transform().rotation() = glm::vec3(x, y, z) * float(180.0/3.14159265);
			}
		}
	}
};
