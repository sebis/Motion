#ifndef COMMON_ANIMATOR
#define COMMON_ANIMATOR

#include "Component.h"
#include "Trace.h"

namespace Common
{
	class GameObject;

	class Animator : public Component
	{
	public:
		Animator(GameObject * gameObject, bool loop = false) : m_gameObject(gameObject), m_loop(loop) {};
		virtual ~Animator() {};

		virtual void update(float dt) {}
		virtual void visualize() {}

	protected:
		GameObject * m_gameObject;
		bool m_loop;
	};
};

#endif /* COMMON_ANIMATOR */
