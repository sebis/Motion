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
#include <map>
#include <vector>

namespace Interpolation
{
	class Common::GameObject;

	template<typename T>
	class KeyframeAnimator : public Common::Animator, public ControlPoints<Keyframe<T> >
	{
	public:
		KeyframeAnimator(Common::GameObject * gameObject, Interpolator<T> * interpolator, T& result, 
			bool reparameterize = false,
			bool connect_end_points = false,
			bool orient_to_tangent = true,
			bool loop = true)
			: Animator(gameObject, loop),
			m_interpolator(interpolator),
			m_renderer(0),
			m_result(result),
			m_time(0),
			m_useArcLength(false),
			m_initialized(false),
			m_subsegments(1000),
			m_reparameterize(reparameterize),
			m_closed(connect_end_points),
			m_orient(orient_to_tangent)
		{}

		virtual ~KeyframeAnimator() {};

		inline const Keyframe<T>& operator[](int i) const
		{
			int index = i;
			// TODO: better way to define closed loops
			if (m_closed) {
				/* In our case if we have a closed loop, the start and end point
				   have the same values but different times so we can't just wrap the index.
				   Depending whether we are at the end or the beginning we need to skip 
				   one of the keys */
				if (i == -1) index = count()-2;   // skip count()-1
				else if (i == count()) index = 1; // skip 0
			} else {
				// For a non-closed loop, just clamp the value so that end points are duplicated
				if (i < 0) index = 0;
				else if (i > count()-1) index = count()-1;
			}

			return m_keyframes[index];
		}

		inline int count() const
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

		inline void setRenderer(SplineRenderer * renderer)
		{
			m_renderer = renderer;
		}

		// get the keyframe index interval [k,k+1] for global parameter t and return value _t that is the local parameter between [k,k+1]
		inline float get_k(float t, int& k) const
		{
			if (m_useArcLength)
				t = s(t);

			assert(0 <= t && t <= 1);

			float time = t * max();

			// get a value k so that k is in the interval [0, count-2]. That way k+1 is always valid
			typename std::vector<Keyframe<T> >::const_iterator low = std::upper_bound(m_keyframes.begin(), m_keyframes.end() - 1, time, typename Keyframe<T>::Less());
			k = int(low - m_keyframes.begin() - 1);

			// interpolate the local paramter linearly based on keyframe times
			float _t = (time - m_keyframes[k].time) / (m_keyframes[k+1].time - m_keyframes[k].time);

			assert(0.0f <= _t && _t <= 1.0f);

			return _t;
			
		}

		inline void visualize()
		{
			if (m_renderer && m_renderer->initialized())
				m_renderer->draw();
		}

		inline const ControlPoints<Keyframe<T> >& keys() const
		{
			return *this;
		}

		void update(float dt);

	private:
		// orients the parent gameobject of this animator parallel to the tangent
		void orient(T tangent);
		
		// find global arc-length parameter s from a pre-calculated table based on t
		inline float s(float t) const
		{
			// find values t0 and t1 so that t0 <= t < t1
			std::map<float,float>::const_iterator it = m_params.upper_bound(t);

			if (it == m_params.end())
				return 1.0f;

			assert(it != m_params.end());

			// get s and t values from table
			float s1 = it->first;
			float t1 = it->second;
			--it;
			float s0 = it->first;
			float t0 = it->second;

			// linearly interpolate between [s0,s1] based on t
			float r = (t - s0) / (s1 - s0);
			float s = t0 + r*(t1 - t0);

			assert(0 <= s0 && s0 <= 1 && 0 <= s1 && s1 <= 1);

			return s;
		}

		inline void reparameterize()
		{
			Trace::info("Reparameterizing..\n");

			// set value to false so that the interpolator uses original t values instead of reparameterized ones
			m_useArcLength = false;
			m_params.clear();

			// delta increment between each sample
			float dt = 1.0f/m_subsegments;

			// calculate total length of the curve
			float s = m_interpolator->arcLengthAt(*this, 1.0f);
			Trace::info("Length: %f\n", s);
			float is = 1.0f/s;

			// for each delta segment calculate the current length and store it in a map
			for (unsigned i = 0; i <= m_subsegments; i++) {
				float ds = m_interpolator->arcLengthAt(*this, i*dt);
				m_params.insert(std::make_pair<float,float>(is*ds, i*dt));
			}

			// notify that reparameterized values can be used
			m_useArcLength = true;
		}


		SplineRenderer * m_renderer;
		// The interpolator used for interpolating values between key frames
		Interpolator<T> * m_interpolator;
		std::vector<Keyframe<T> > m_keyframes;

		// flag to notify if the current parameterization should use original or arc-length
		bool m_useArcLength;
		// flag if the owner game object should be rotated towards the tangent of the curve
		bool m_orient;
		// tells if the first and last keyframes should be connected
		bool m_closed;
		// value to indicate that the curve should be reparameterized
		bool m_reparameterize;

		bool m_initialized;
		std::map<float, float> m_params;

		// result of the animation and interpolation should be updated here
		T& m_result;

		float m_time;

		// number of subsegments to use when calculating the arc-length approximation between key frames
		const unsigned m_subsegments;
	};

	// declare template function specialization
	template<>
	void KeyframeAnimator<glm::vec3>::orient(glm::vec3 tangent);

	// define template function for all other types
	template<typename T>
	void KeyframeAnimator<T>::orient(T tangent)
	{
	}

	template<typename T>
	void KeyframeAnimator<T>::update(float dt)
	{
		// TODO: kind of a hack..
		if (!m_initialized) {
			m_initialized = true;

			if (m_reparameterize)
				reparameterize();
			
			if (m_renderer)
				m_renderer->init();
		}

		// wrap around if loop is enabled
		m_time = m_loop ? std::fmod(m_time, max()) : std::min(m_time, max());

		// map m_time to interval [0,1] based on first and last key frames
		float t = (m_time - m_keyframes[0].time) / (m_keyframes[count() - 1].time - m_keyframes[0].time);

		// get the index k of the key frame where t lies between [k,k+1]
		int k = -1;
		float _t = get_k(t, k);

		// calculate tangent if we want to orient the animated object according to the tangent
		if (m_orient)
		{
			T tangent;
			m_interpolator->interpolate(m_result, *this, k, _t, &tangent);
			orient(tangent);
		}
		else
		{
			m_interpolator->interpolate(m_result, *this, k, _t);
		}

		// accumulate time
		m_time += dt;
	}

} /* namespace Interpolation */

#endif /* INTERPOLATION_KEYFRAME */
