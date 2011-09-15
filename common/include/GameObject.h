#ifndef COMMON_GAMEOBJECT
#define COMMON_GAMEOBJECT

#include "Animator.h"
#include "Transform.h"
#include "Renderer.h"

namespace Common
{
	class GameObject
	{
	public:
		GameObject();
		virtual ~GameObject();

		Transform m_transform;
		Animator * m_animator;
		Renderer * m_renderer;
	};
};

#endif /* COMMON_GAMEOBJECT */