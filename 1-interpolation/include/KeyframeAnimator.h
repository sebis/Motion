#ifndef INTERPOLATION_KEYFRAME
#define INTERPOLATION_KEYFRAME

#include "Animator.h"
#include "ControlPoints.h"
#include "Interpolator.h"
#include "SplineRenderer.h"
#include "Trace.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>
#include <algorithm>
#include <functional>
#include <map>
#include <vector>

namespace Interpolation
{
	class Common::GameObject;

	template<typename T>
	class KeyframeAnimator : public Common::Animator, public ControlPoints<Keyframe<T>>
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

		inline void setRenderer(SplineRenderer * renderer)
		{
			m_renderer = renderer;
		}

		inline float get_k(float t, int& k) const
		{
			if (m_useArcLength)
				t = s(t);

			assert(0 <= t && t <= 1);
			float time = t * max();

			typename std::vector<Keyframe<T>>::const_iterator low = std::upper_bound(m_keyframes.begin(), m_keyframes.end() - 1, time, Keyframe<T>::Less());
			k = int(low - m_keyframes.begin() - 1);

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
		void orient(T tangent) {}
		
		inline float s(float t) const
		{
			std::map<float,float>::const_iterator it = m_params.upper_bound(t);

			if (it == m_params.end())
				return 1.0f;

			//return it->second;

			assert(it != m_params.end());

			float s1 = it->first;
			float t1 = it->second;
			--it;
			float s0 = it->first;
			float t0 = it->second;

			float r = (t - s0) / (s1 - s0);
			float s = t0 + r*(t1 - t0);

			//return 0.5 * (s0  + s1);

			assert(0 <= s0 && s0 <= 1 && 0 <= s1 && s1 <= 1);
			//Trace::info("%f < %f < %f\n", s0, t, s1);
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
		Interpolator<T> * m_interpolator;
		std::vector<Keyframe<T>> m_keyframes;

		bool m_useArcLength;
		bool m_orient;
		bool m_closed;
		bool m_reparameterize;
		bool m_initialized;
		std::map<float, float> m_params;

		T& m_result;

		float m_time;
		const unsigned m_subsegments;
	};

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
		m_time = m_loop ? std::fmodf(m_time, max()) : std::min(m_time, max());

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

} /* namespace Interpolation */

#endif /* INTERPOLATION_KEYFRAME */
