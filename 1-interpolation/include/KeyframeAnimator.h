#ifndef INTERPOLATION_KEYFRAME
#define INTERPOLATION_KEYFRAME

#include "Animator.h"
#include "Interpolator.h"

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
		KeyframeAnimator(Common::GameObject * gameObject, Interpolator<T> * interpolator)
			: Animator(gameObject), m_interpolator(interpolator) {};

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
	};

	typedef KeyframeAnimator<Common::Transform> TransformKeyframeAnimator;

	/*class TransformKeyframeAnimator : public KeyframeAnimator<Common::Transform>
	{
	public:
		TransformKeyframeAnimator(Common::GameObject * gameObject, Interpolator<Common::Transform> * interpolator)
			: KeyframeAnimator<Common::Transform>(gameObject, interpolator)
		{
		}

		void update(float dt);
	};*/
};

#endif /* INTERPOLATION_KEYFRAME */