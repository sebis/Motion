#ifndef INTERPOLATION_INTERPOLATOR
#define INTERPOLATION_INTERPOLATOR

#include "ControlPoints.h"

namespace Interpolation
{
	template<typename T>
	class Interpolator
	{
	public:
		typedef const ControlPoints<Keyframe<T>>& Keys;

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

		virtual void interpolate(T& out, Keys keys, int k, float t) = 0;
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
	class CatmullRomInterpolator : public Interpolator<T>
	{
	public:
		void interpolate(T& out, Keys keys, int k, float t)
		{
			const float& h00 = (1 + 2 * t) * (1 - t) * (1 - t);
			const float& h10 = t * (1 - t) * (1 - t);
			const float& h01 = t * t * (3 - 2 * t);
			const float& h11 = t * t * (t - 1);

			const T& p0 = keys[std::max(0, k-1)].value;
			const T& p1 = keys[k].value;
			const T& p2 = keys[k+1].value;
			const T& p3 = keys[std::min(keys.count()-1, k+2)].value;

			const float t0 = std::max(0, k-1), t1 = k, t2 = k+1, t3 = std::min(keys.count()-1, k+2);

			const T& m1 = (p2 - p0) / float(t2 - t0);
			const T& m2 = (p3 - p1) / float(t3 - t1);

			out = h00 * p1 + h10 * m1 + h01 * p2 + h11 * m2;
		}

		virtual float arcLength(Keys keys, int k1, int k2, float _t = 1.0f)
		{
			return 0.0f;
		}
	};

	template<typename T>
	class KochanekBartelsInterpolator : public Interpolator<T>
	{
	public:
		void interpolate(T& out, Keys keys, int k, float _t)
		{
			// TODO: could do some precalculations
			const float& h00 = (1 + 2 * _t) * (1 - _t) * (1 - _t);
			const float& h10 = _t * (1 - _t) * (1 - _t);
			const float& h01 = _t * _t * (3 - 2 * _t);
			const float& h11 = _t * _t * (_t - 1);

			const float& t = keys[k].tension;
			const float& b = keys[k].bias;
			const float& c = keys[k].continuity;

			const T& p0 = keys[std::max(0, k-1)].value;
			const T& p1 = keys[k].value;
			const T& p2 = keys[k+1].value;
			const T& p3 = keys[std::min(keys.count()-1, k+2)].value;

			//const float t0 = keys[std::max(0, k-1)].time, t1 = keys[k].time, t2 = keys[k+1].time, t3 = keys[std::min(keys.count()-1, k+2)].time;
			const float t0 = std::max(0, k-1), t1 = k, t2 = k+1, t3 = std::min(keys.count()-1, k+2);

			float dt1 = 2 * (t2 - t1) / (t2 - t0);
			float dt2 = 2 * (t2 - t1) / (t3 - t1);

			const T& m1 = (((1-t)*(1-b)*(1-c)/2) * (p2 - p1) + ((1-t)*(1+b)*(1+c)/2) * (p1 - p0)) * dt1; // in
			const T& m2 = (((1-t)*(1-b)*(1+c)/2) * (p3 - p2) + ((1-t)*(1+b)*(1-c)/2) * (p2 - p1)) * dt2; // out

			out = h00 * p1 + h10 * m1 + h01 * p2 + h11 * m2;
		}
	};
};

#endif /* INTERPOLATION_INTERPOLATOR */
