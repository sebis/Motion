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

		virtual void interpolate(T& out, Keys keys, int k, float t, T* tangent = 0) = 0;

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

			// calculate whole arcs between control points
			for (int i = 0; i < k; i++) {
				length += arcLength(keys, i, i+1);
			}

			// calculate the correct portion _t of the last segment
			if (_t != 0)
				length += arcLength(keys, k, k+1, _t);

			return length;
		}
	};

	template<typename T>
	class LinearInterpolator : public Interpolator<T>
	{
	public:
		void interpolate(T& out, Keys keys, int k, float _t, T* tangent = 0)
		{
			const T& p1 = keys[k].value;
			const T& p2 = keys[k+1].value;

			out = p1 + _t * (p2 - p1);

			if (tangent)
				*tangent = glm::normalize(p2 - p1);
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
		virtual void interpolate(T& out, Keys keys, int k, float _t, T* tangent = 0)
		{
			const T& q1 = glm::normalize(keys[k].value);
			const T& q2 = glm::normalize(keys[k+1].value);

			out = Slerp(q1, q2, _t);
		}

		virtual float arcLength(Keys keys, int k1, int k2, float _t = 1.0f)
		{
			// TODO: implement slerp arclength
			return 0.0f;
		}

	protected:
		T Slerp(const T& p, const T& q, float t)
		{
			// p and q should be unit quaternions
			float omega = std::acos(glm::dot(p, q));
			return (std::sin((1 - t) * omega)*p + std::sin(t*omega)*q)/std::sin(omega);
		}
	};

	template<typename T>
	class BezierSphericalLinearInterpolator : public SphericalLinearInterpolator<T>
	{
	public:
		void interpolate(T& out, Keys keys, int k, float _t, T* tangent = 0)
		{
			const T& q0 = glm::normalize(keys[k-1].value);
			const T& q1 = glm::normalize(keys[k].value);
			const T& q2 = glm::normalize(keys[k+1].value);
			const T& q3 = glm::normalize(keys[k+2].value);

			// Shoemaker '84
			const T& a1 = Bisect(Double(q0, q1), q2);
			const T& b1 = Double(a1, q1);
			const T& a2 = Bisect(Double(q1, q2), q3);
			const T& b2 = Double(a2, q2);

			// Calculate result by using de Castlejau's algorithm for 3rd order Bezier curve
			const T& p00 = q1;
			const T& p10 = a1;
			const T& p20 = b2;
			const T& p30 = q2;

			const T& p01 = Slerp(p00, p10, _t);
			const T& p11 = Slerp(p10, p20, _t);
			const T& p21 = Slerp(p20, p30, _t);

			const T& p02 = Slerp(p01, p11, _t);
			const T& p12 = Slerp(p11, p21, _t);

			out = Slerp(p02, p12, _t);
		}

		float arcLength(Keys keys, int k1, int k2, float _t = 1.0f)
		{
			// TODO: implement bezier slerp arclength
			return 0.0f;
		}

	private:
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
		void interpolate(T& out, Keys keys, int k, float t, T* tangent = 0)
		{
			const T& p0 = keys[k-1].value;
			const T& p1 = keys[k].value;
			const T& p2 = keys[k+1].value;
			const T& p3 = keys[k+2].value;

			/* Calculated from matrix form
			   out = 0.5 * [ t^3 t^2 t 1 ] * [ -1  3 -3  1 ] * [ p0 ]
                                             [  2 -5  4 -1 ]   [ p1 ]
                                             [ -1  0  1  0 ]   [ p2 ]
                                             [  0  2  0  0 ]   [ p3 ]

			   where the matrix is the basis matrix for Catmull-Rom splines
			*/
			out = 0.5f * ((-p0 + 3.0f*p1 - 3.0f*p2 + p3)*t*t*t +
				(2.0f*p0 - 5.0f*p1 + 4.0f*p2 - p3)*t*t +
				(-p0 + p2)*t +
				2.0f*p1);

			if (tangent)
			{
				// The derivate of the spline respect to t
				*tangent = 0.5f * ((-3.0f*p0 + 9.0f*p1 - 9.0f*p2 + 3.0f*p3)*t*t +
					(4.0f*p0 - 10.0f*p1 + 8.0f*p2 - 2.0f*p3)*t +
					(-p0 + p2));
			}
		}

		virtual float arcLength(Keys keys, int k1, int k2, float _t = 1.0f)
		{
			// TODO: calculate integral approximation
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
	class KochanekBartelsInterpolator : public CatmullRomInterpolator<T>
	{
	public:
		void interpolate(T& out, Keys keys, int k, float _t, T* tangent = 0)
		{
			const float& t = keys[k].tension;
			const float& b = keys[k].bias;
			const float& c = keys[k].continuity;

			const T& p0 = keys[k-1].value;
			const T& p1 = keys[k].value;
			const T& p2 = keys[k+1].value;
			const T& p3 = keys[k+2].value;

			// calculate incoming and outgoing tangents
			const T& d1 = (0.5f*(1-t)*(1-b)*(1-c)) * (p2 - p1) + (0.5f*(1-t)*(1+b)*(1+c)) * (p1 - p0); // in
			const T& d2 = (0.5f*(1-t)*(1-b)*(1+c)) * (p3 - p2) + (0.5f*(1-t)*(1+b)*(1-c)) * (p2 - p1); // out

			/* Calculated from matrix form
			   out = [ t^3 t^2 t 1 ] * [  2 -2  1  1 ] * [ p1 ]
                                       [ -3  3 -2 -1 ]   [ p2 ]
                                       [  0  0  1  0 ]   [ d1 ]
                                       [  1  0  0  0 ]   [ d2 ]

			   where the matrix is the coefficient matrix for cubic hermite splines
			*/
			out = ((2.0f*p1 - 2.0f*p2 + d1 + d2)*_t*_t*_t +
				(-3.0f*p1 + 3.0f*p2 - 2.0f*d1 - d2)*_t*_t +
				(d1)*_t +
				p1);

			if (tangent)
			{
				// The derivate of the spline respect to t
				*tangent = ((6.0f*p1 - 6.0f*p2 + 3.0f*d1 + 3.0f*d2)*_t*_t +
					(-6.0f*p1 + 6.0f*p2 - 4.0f*d1 - 2.0f*d2)*_t +
					d1);
			}
		}
	};
}

#endif /* INTERPOLATION_INTERPOLATOR */
