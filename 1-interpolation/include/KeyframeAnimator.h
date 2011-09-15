#ifndef INTERPOLATION_KEYFRAME
#define INTERPOLATION_KEYFRAME

#include "Animator.h"
#include "Interpolator.h"
#include "Trace.h"

#include <algorithm>
#include <map>

namespace Interpolation
{
	class Common::GameObject;

	template<typename T>
	class KeyframeAnimator : public Common::Animator
	{
	protected:
		typedef std::pair<const float, T> Keyframe;

	public:
		KeyframeAnimator(Common::GameObject * gameObject, Interpolator<T> * interpolator, T& result)
			: Animator(gameObject), m_interpolator(interpolator), m_result(result) {};

		virtual ~KeyframeAnimator() {};

		virtual void update(float dt);

		inline std::pair<Keyframe, Keyframe> getKeyframes(float time)
		{
			if (m_keyframes.empty())
				throw 0;

			std::map<float, T>::iterator it = m_keyframes.lower_bound(time);

			if (it == m_keyframes.end())
				return std::make_pair(*m_keyframes.rbegin(), *m_keyframes.rbegin());
			else if (it == m_keyframes.begin())
				return std::make_pair(*m_keyframes.begin(), *m_keyframes.begin());
			else
				return std::make_pair(*(--it), *(it));
		}

		inline void addKeyframe(float time, T value)
		{
			Keyframe keyframe(time, value);
			m_keyframes.insert(keyframe);
		}

	protected:
		Interpolator<T> * m_interpolator;

	private:
		std::map<float, T> m_keyframes;
		T& m_result;
	};

	typedef KeyframeAnimator<glm::vec3> LocationKeyframeAnimator;
	typedef KeyframeAnimator<Common::Transform> TransformKeyframeAnimator;

	template<typename T>
	void KeyframeAnimator<T>::update(float dt)
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
	}
};

#endif /* INTERPOLATION_KEYFRAME */