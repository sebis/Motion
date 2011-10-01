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
		KeyframeAnimator(Common::GameObject * gameObject, Interpolator<T> * interpolator, T& result, bool loop = false)
			: Animator(gameObject, loop), m_interpolator(interpolator), m_renderer(0), m_result(result), m_time(0), m_parameterize(false)
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

		inline float get_k(float t, int& k) const
		{
			if (m_parameterize)
				t = s(t);

			assert(0 <= t && t <= 1);
			float time = t * max();

			typename std::vector<Keyframe<T>>::const_iterator low = std::upper_bound(m_keyframes.begin(), m_keyframes.end() - 1, time, Keyframe<T>::Less());
			k = int(low - m_keyframes.begin() - 1);

			float _t = (time - m_keyframes[k].time) / (m_keyframes[k+1].time - m_keyframes[k].time);
			assert(0.0f <= _t && _t <= 1.0f);
			return _t;
			
		}

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
			// TODO: obsolete call
			m_interpolator->reparameterize(*this);

			const int size = 100;
			float dt = 1.0f/size;

			float s = m_interpolator->arcLengthAt(*this, 1.0f);
			Trace::info("Length: %f\n", s);

			for (int i = 0; i <= size; i++) {
				float ds = m_interpolator->arcLengthAt(*this, i*dt);
				m_params.insert(std::make_pair<float,float>(ds/s, i*dt));
				Trace::info("%f %f\n", i*dt, ds/s);
			}

			m_parameterize = true;
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

		bool m_parameterize;
		std::map<float, float> m_params;

		T& m_result;

		float m_time;
	};

	template<typename T>
	void KeyframeAnimator<T>::update(float dt)
	{
		m_time = m_loop ? std::fmodf(m_time, max()) : std::min(m_time, max());

		/*typename std::vector<Keyframe<T>>::iterator low = std::upper_bound(m_keyframes.begin(), m_keyframes.end() - 1, m_time, Keyframe<T>::Less());
		int k = int(low - m_keyframes.begin() - 1);*/

		float t = (m_time - m_keyframes[0].time) / (m_keyframes[m_keyframes.size() - 1].time - m_keyframes[0].time);

		int k = -1;
		float _t = get_k(t, k);

		// TODO: calculate t by a s(t), integrate curve length
		//float _t = (m_time - m_keyframes[k].time) / (m_keyframes[k+1].time - m_keyframes[k].time);
		

		//if (m_parameterize)
			//t = s(t);
		T before = m_result;
		
		m_interpolator->interpolate(m_result, *this, k, _t);

		T after = m_result;

		if (after != before) {
			T start = glm::vec3(0, 0, 1);
			T tangent = glm::normalize(after - before);
			
			float z = glm::dot(start, tangent);
			glm::vec3 axis = glm::normalize(glm::cross(start, tangent));
			glm::vec3 right = glm::cross(axis, start);
			float x = glm::dot(right, tangent);
			//Trace::info("forward: %f %f %f\n", forward.x, forward.y, forward.z);

			float theta = std::atan2(x, z);

			glm::vec3 u = axis;

			/*glm::mat3 uu(u.x * u.x, u.x*u.y, u.x*u.z,
				u.x*u.y, u.y*u.y, u.y*u.z,
				u.x*u.z, u.y*u.z, u.z*u.z);
			glm::mat3 ux(0, -u.z, u.y,
				u.z, 0, -u.x,
				-u.y, u.x, 0);*/

			glm::mat3 uu(u.x*u.x, u.x*u.y, u.x*u.z,
				u.x*u.y, u.y*u.y, u.y*u.z,
				u.x*u.z, u.y*u.z, u.z*u.z);
			glm::mat3 ux(0, u.z, -u.y,
				-u.z, 0, u.x,
				u.y, -u.x, 0);

			glm::mat3 r = glm::mat3(1.0f) * glm::cos(theta) + glm::sin(theta)*ux + (1 - glm::cos(theta))*uu;


			/* ATLEAST Y WORKS:
			float yaw = std::asin(-r[1][2]) * float(180.0/M_PI);
			float pitch = std::atan2(r[0][2], r[2][2]) * float(180.0/M_PI);
			float roll = std::atan2(r[1][0], r[1][1]) * float(180.0/M_PI);*/

			float yaw = std::asin(-r[1][2]) * float(180.0/M_PI);
			float pitch = std::atan2(-r[0][2], r[2][2]) * float(180.0/M_PI);
			float roll = std::atan2(r[1][0], r[1][1]) * float(180.0/M_PI);

			Trace::info("%f %f %f\n", yaw, pitch, roll);
			m_gameObject->m_transform.rotation() = glm::vec3(yaw, pitch, roll);
			m_gameObject->m_transform.rot() = r;
		}

		m_time += dt;
	}
};

#endif /* INTERPOLATION_KEYFRAME */
