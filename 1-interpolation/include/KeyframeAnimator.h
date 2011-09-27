#ifndef INTERPOLATION_KEYFRAME
#define INTERPOLATION_KEYFRAME

#include "Animator.h"
#include "ControlPoints.h"
#include "Interpolator.h"
#include "SplineRenderer.h"
#include "Trace.h"

#include <cassert>
#include <algorithm>
#include <functional>
#include <vector>

namespace Interpolation
{
	class Common::GameObject;

	template<typename T>
	class KeyframeAnimator : public Common::Animator, public ControlPoints<Keyframe<T>>
	{
	public:
		KeyframeAnimator(Common::GameObject * gameObject, Interpolator<T> * interpolator, T& result, bool loop = false)
			: Animator(gameObject, loop), m_interpolator(interpolator), m_renderer(0), m_result(result), m_time(0)
		{}

		virtual ~KeyframeAnimator() {};

		inline const Keyframe<T>& operator[](const int& i) const
		{
			return m_keyframes[i];
		}

		inline Keyframe<T>& operator[](const int& i)
		{
			return m_keyframes[i];
		}

		inline const int count() const
		{
			return m_keyframes.size();
		}

		inline const float max() const
		{
			return std::max_element(m_keyframes.begin(), m_keyframes.end())->time;
		}

		inline void addKeyframe(float time, T value, float t = 0, float b = 0, float c = 0)
		{
			Keyframe<T> keyframe(time, value, t, b, c);
			m_keyframes.push_back(keyframe);
		}

		inline void setRenderer(Common::Renderer * renderer)
		{
			m_renderer = renderer;
		}

		inline void reparameterize()
		{
			m_interpolator->reparameterize(*this);
		}

		inline void visualize()
		{
			if (m_renderer)
				m_renderer->draw();
		}

		void update(float dt);

	private:
		Common::Renderer * m_renderer;
		Interpolator<T> * m_interpolator;
		std::vector<Keyframe<T>> m_keyframes;

		T& m_result;

		float m_time;
	};

	template<typename T>
	void KeyframeAnimator<T>::update(float dt)
	{
		m_time = m_loop ? std::fmodf(m_time, max()) : std::min(m_time, max());

		typename std::vector<Keyframe<T>>::iterator low = std::upper_bound(m_keyframes.begin(), m_keyframes.end() - 1, m_time, Keyframe<T>::Less());

		int k = int(low - m_keyframes.begin() - 1);
		// TODO: calculate t by a s(t), integrate curve length
		float t = (m_time - m_keyframes[k].time) / (m_keyframes[k+1].time - m_keyframes[k].time);
		m_interpolator->interpolate(m_result, *this, k, t);

		m_time += dt;
	}
};

#endif /* INTERPOLATION_KEYFRAME */
