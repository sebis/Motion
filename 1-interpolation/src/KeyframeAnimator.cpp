#include "GameObject.h"
#include "KeyframeAnimator.h"
#include "Transform.h"
#include "Trace.h"

#include "glm/glm.hpp"

using namespace Common;

namespace Interpolation
{
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