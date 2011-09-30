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
		virtual float arcLengthAt(Keys keys, float t) = 0;
		virtual void reparameterize(ControlPoints<Keyframe<T>>& keys) {}
		virtual void interpolate(T& out, Keys keys, int k, float t) = 0;
	};

	template<typename T>
	class LinearInterpolator : public Interpolator<T>
	{
	public:
		void interpolate(T& out, Keys keys, int k, float _t)
		{
			//float _t = keys.get_k(t, k);
			//t = keys.s(t);

			const T& p1 = keys[k].value;
			const T& p2 = keys[k+1].value;

			//t = keys.s(t);
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

		float arcLengthAt(Keys keys, float t)
		{
			float length = 0;
			if (keys.count() < 2)
				return length;

			if (t >= 0.1f)
				int temp = 0;
			int N1 = keys.count() - 1;
			float N1t = N1 * t;
			int k = 0;
			float _t = keys.get_k(t, k);

			for (int i = 0; i < k; i++) {
				length += arcLength(keys, i, i+1);
			}
			if (_t != 0)
				length += arcLength(keys, k, k+1, _t);

			return length;
		}

		void reparameterize(ControlPoints<Keyframe<T>>& keys)
		{
			//Trace::info("Full length: %f\n", arcLengthAt(keys, 1.0f));
			/*float t = keys[keys.count() - 1].time - keys[0].time;
			float s = 0;

			for (int i = 0; i < keys.count() - 1; i++) {
				keys[i].time = s;

				float s_i = arcLength(keys, i, i+1, 1.0f);
				s += s_i;
			}

			float r = t/s;

			for (int i = 0; i < keys.count() - 1; i++) {
				keys[i].time *= r;
			}*/
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
	};

	template<typename T>
	class KochanekBartelsInterpolator : public Interpolator<T>
	{
	public:
		void interpolate(T& out, Keys keys, int k, float t)
		{
			const float& h00 = (1 + 2 * t) * (1 - t) * (1 - t);
			const float& h10 = t * (1 - t) * (1 - t);
			const float& h01 = t * t * (3 - 2 * t);
			const float& h11 = t * t * (t - 1);

			{
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
		}
	};
};

#endif /* INTERPOLATION_INTERPOLATOR */
