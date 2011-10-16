#include "GameObject.h"
#include "KeyframeAnimator.h"
#include "Transform.h"
#include "Trace.h"

#include "glm/glm.hpp"

using namespace Common;

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

				m_gameObject->m_transform.rotation() = glm::vec3(x, y, z) * float(180.0/M_PI);
			}
		}
	}
	/*template<typename T>
	void KeyframeAnimator::update(float dt)
	{
		static float time = 0;

		try {
			std::pair<Keyframe, Keyframe> bounds = getKeyframes(time);

			Keyframe first = bounds.first;
			Keyframe second = bounds.second;

			float t;
			if (first.first == second.first)
				t = 1;
			else
				t = (time - first.first) / (second.first - first.first);
			
			Trace::info("Interpolating with: %f {%f, %f} / %f\n", t, first.first, second.first, time);

			//m_gameObject->m_transform.m_position = m_interpolator->interpolate(first.second, second.second, t);
			m_interpolator->interpolate2(m_result, first.second, second.second, t);


		} catch ( ... )
		{
			Trace::warning("No key frames found\n");
		}

		time += dt;
	}*/
};
