#ifndef INTERPOLATION_INTERPOLATOR
#define INTERPOLATION_INTERPOLATOR

namespace Interpolation
{
	template<typename T>
	class Interpolator
	{
	public:
		virtual T interpolate(T first, T second, float t) = 0;
	};

	template<typename T>
	class LinearInterpolator : public Interpolator<T>
	{
	public:
		T interpolate(T first, T second, float t);
	};
};

#endif /* INTERPOLATION_INTERPOLATOR */