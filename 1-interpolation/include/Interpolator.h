#ifndef INTERPOLATION_INTERPOLATOR
#define INTERPOLATION_INTERPOLATOR

#include "ControlPoints.h"

#include "glm/gtc/quaternion.hpp"

#include <cassert>

namespace Interpolation
{
	template<typename T>
	class Interpolator
	{
	public:
		typedef const ControlPoints<Keyframe<T>>& Keys;

		virtual void interpolate(T& out, Keys keys, int k, float t) = 0;

		T interpolate(Keys keys, int k, float t)
		{
			T tmp;
			interpolate(tmp, keys, k, t);
			return tmp;
		}

		virtual float arcLength(Keys keys, int k1, int k2, float _t = 1.0f) = 0;

		float arcLengthAt(Keys keys, float t)
		{
			float length = 0;
			if (keys.count() < 2)
				return length;

			int k = 0;
			float _t = keys.get_k(t, k);

			for (int i = 0; i < k; i++) {
				length += arcLength(keys, i, i+1);
			}
			if (_t != 0)
				length += arcLength(keys, k, k+1, _t);

			return length;
		}
	};

	template<typename T>
	class LinearInterpolator : public Interpolator<T>
	{
	public:
		void interpolate(T& out, Keys keys, int k, float _t)
		{
			const T& p1 = keys[k].value;
			const T& p2 = keys[k+1].value;

			out = p1 + _t * (p2 - p1);
		}

		float arcLength(Keys keys, int k1, int k2, float _t = 1.0f)
		{
			const T& p1 = keys[k1].value;
			const T& p2 = keys[k2].value;

			assert(0.0f <= _t && _t <= 1.0f);
			// euclidean distance
			const T& p = p1 + _t * (p2 - p1);

			return glm::distance(p, p1);
		}
	};

	template<typename T>
	class SphericalLinearInterpolator : public Interpolator<T>
	{
	public:
		void interpolate(T& out, Keys keys, int k, float _t)
		{
			const glm::quat& q1 = glm::normalize(keys[k].value);
			const glm::quat& q2 = glm::normalize(keys[k+1].value);

			float omega = std::acos(glm::dot(q1, q2));
			out = (std::sin((1 - _t) * omega)*q1 + std::sin(_t*omega)*q2)/std::sin(omega);
		}

		float arcLength(Keys keys, int k1, int k2, float _t = 1.0f)
		{
			// TODO: implement slerp arclength
			return 0.0f;
		}
	};

	template<typename T>
	class BezierSphericalLinearInterpolator : public Interpolator<T>
	{
	public:
		void interpolate(T& out, Keys keys, int k, float _t)
		{
			const glm::quat& q0 = keys[std::max(0, k-1)].value;
			const glm::quat& q1 = keys[k].value;
			const glm::quat& q2 = keys[k+1].value;
			const glm::quat& q3 = keys[std::min(keys.count()-1, k+2)].value;


			const glm::quat& a1 = Bisect(Double(q0, q1), q2);
			const glm::quat& b1 = Double(a1, q1);
			const glm::quat& a2 = Bisect(Double(q1, q2), q3);
			const glm::quat& b2 = Double(a2, q2);

			const glm::quat& p00 = q1; //
			const glm::quat& p10 = a1; //
			const glm::quat& p20 = b2; //
			const glm::quat& p30 = q2; //

			const glm::quat& p01 = Slerp(p00, p10, _t); //
			const glm::quat& p11 = Slerp(p10, p20, _t); //
			const glm::quat& p21 = Slerp(p20, p30, _t); //

			const glm::quat& p02 = Slerp(p01, p11, _t); //
			const glm::quat& p12 = Slerp(p11, p21, _t); //

			out = Slerp(p02, p12, _t); //
		}

		float arcLength(Keys keys, int k1, int k2, float _t = 1.0f)
		{
			// TODO: implement slerp arclength
			return 0.0f;
		}

	private:
		T Slerp(const T&  p, const T& q, float t)
		{
			float omega = std::acos(glm::dot(q, p));
			return (std::sin((1 - t) * omega)*q + std::sin(t*omega)*p)/std::sin(omega);
		}

		T Bisect(const T& p, const T& q)
		{
			return glm::normalize(p + q);
		}

		T Double(const T& p, const T& q)
		{
			return 2 * glm::dot(p, q) * q + (-p);
		}
	};

	template<typename T>
	class CatmullRomInterpolator : public Interpolator<T>
	{
	public:
		void interpolate(T& out, Keys keys, int k, float t)
		{
			/*const float& h00 = (1 + 2 * t) * (1 - t) * (1 - t);
			const float& h10 = t * (1 - t) * (1 - t);
			const float& h01 = t * t * (3 - 2 * t);
			const float& h11 = t * t * (t - 1);

			const T& p0 = keys[std::max(0, k-1)].value;
			const T& p1 = keys[k].value;
			const T& p2 = keys[k+1].value;
			const T& p3 = keys[std::min(keys.count()-1, k+2)].value;

			const float t0 = std::max(0, k-1), t1 = k, t2 = k+1, t3 = std::min(keys.count()-1, k+2);

			const T& m1 = (p2 - p0) / float(t2 - t0);
			const T& m2 = (p3 - p1) / float(t3 - t1);*/

			// duplicate end points if necessary
			// TODO: could also reflect
			const T& p0 = keys[std::max(0, k-1)].value;
			const T& p1 = keys[k].value;
			const T& p2 = keys[k+1].value;
			const T& p3 = keys[std::min(keys.count()-1, k+2)].value;

			const glm::vec4 T(t*t*t, t*t, t, 1);

			// Catmull-Rom basis in matrix form
			const glm::mat4 B = 0.5f * glm::mat4(
				-1,  3, -3,  1,
				 2, -5,  4, -1,
				-1,  0,  1,  0,
				 0,  2,  0,  0);

			// for some reason GLM hasn't implemented 4x4 * 4x3 multiplication, so use an extended matrix
			const glm::mat4x4 P(glm::mat4x3(p0, p1, p2, p3));

			// GLM treats matrices in column major order so flip multiplication order
			out = glm::vec3(P * B * T);
		}

		virtual float arcLength(Keys keys, int k1, int k2, float _t = 1.0f)
		{
			const int segments = 100;
			const float dt = _t / segments;
			T prev, current;

			float len = 0.0f;

			interpolate(prev, keys, k1, 0.0f);

			for (int i = 1; i <= segments; i++) {
				interpolate(current, keys, k1, i*dt);

				len += glm::distance(prev, current);

				prev = current;
			}

			return len;
		}
	};

	template<typename T>
	class KochanekBartelsInterpolator : public Interpolator<T>
	{
	public:
		void interpolate(T& out, Keys keys, int k, float _t)
		{
			// TODO: could do some precalculations
			/*const float& h00 = (1 + 2 * _t) * (1 - _t) * (1 - _t);
			const float& h10 = _t * (1 - _t) * (1 - _t);
			const float& h01 = _t * _t * (3 - 2 * _t);
			const float& h11 = _t * _t * (_t - 1);*/

			const float& t = keys[k].tension;
			const float& b = keys[k].bias;
			const float& c = keys[k].continuity;

			const T& p0 = keys[std::max(0, k-1)].value;
			const T& p1 = keys[k].value;
			const T& p2 = keys[k+1].value;
			const T& p3 = keys[std::min(keys.count()-1, k+2)].value;

			//const float t0 = keys[std::max(0, k-1)].time, t1 = keys[k].time, t2 = keys[k+1].time, t3 = keys[std::min(keys.count()-1, k+2)].time;
			/*const float t0 = std::max(0, k-1), t1 = k, t2 = k+1, t3 = std::min(keys.count()-1, k+2);

			float dt1 = 2 * (t2 - t1) / (t2 - t0);
			float dt2 = 2 * (t2 - t1) / (t3 - t1);

			const T& m1 = (((1-t)*(1-b)*(1-c)/2) * (p2 - p1) + ((1-t)*(1+b)*(1+c)/2) * (p1 - p0)) * dt1; // in
			const T& m2 = (((1-t)*(1-b)*(1+c)/2) * (p3 - p2) + ((1-t)*(1+b)*(1-c)/2) * (p2 - p1)) * dt2; // out

			out = h00 * p1 + h10 * m1 + h01 * p2 + h11 * m2;*/

			const glm::vec4 TT(_t*_t*_t, _t*_t, _t, 1);

			// Catmull-Rom basis in matrix form
			const glm::mat4 B(
				2, -2, 1, 1,
				-3, 3, -2, -1,
				0, 0, 1, 0,
				1, 0, 0, 0);

			const T& d1 = (0.5f*(1-t)*(1-b)*(1-c)) * (p2 - p1) + (0.5f*(1-t)*(1+b)*(1+c)) * (p1 - p0); // in
			const T& d2 = (0.5f*(1-t)*(1-b)*(1+c)) * (p3 - p2) + (0.5f*(1-t)*(1+b)*(1-c)) * (p2 - p1); // out

			// for some reason GLM hasn't implemented 4x4 * 4x3 multiplication, so use an extended matrix
			const glm::mat4x4 P(glm::mat4x3(p1, p2, d1, d2));

			// GLM treats matrices in column major order so flip multiplication order
			out = glm::vec3(P * B * TT);
		}
	};
}

#endif /* INTERPOLATION_INTERPOLATOR */
