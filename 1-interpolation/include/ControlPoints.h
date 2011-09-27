#ifndef INTERPOLATION_CONTROLPOINTS
#define INTERPOLATION_CONTROLPOINTS

namespace Interpolation
{
	template<typename T>
	class ControlPoints
	{
	public:
		virtual ~ControlPoints() {}
		virtual const T& operator [](const int& i) const = 0;
		virtual T& operator[](const int& i) = 0;
		virtual const int count() const = 0;
	};

	template<typename T, typename P = float>
	struct Keyframe
	{
		Keyframe(float _time, T _value, P _tension = 0, P _bias = 0, P _continuity = 0) 
			: time(_time), value(_value), tension(_tension), bias(_bias), continuity(_continuity) {}

		inline bool operator< (const Keyframe<T>& b) const { return time < b.time; }
		
		struct Less {
			bool operator() (const float& a, const Keyframe<T>& b) { return a < b.time; }
		};

		float time;
		T value;

		// TODO: more generic params?
		P tension;
		P bias;
		P continuity;
	};
};

#endif /* INTERPOLATION_CONTROLPOINTS */