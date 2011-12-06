#ifndef COMMON_COMPONENT
#define COMMON_COMPONENT

namespace Common
{
	class Component
	{
	public:
		Component() {}
		virtual ~Component() {}

		virtual void update(float /*dt*/) {}
		virtual void draw() {}
	};
}

#endif /* COMMON_COMPONENT */
