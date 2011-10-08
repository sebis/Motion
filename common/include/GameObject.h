#ifndef COMMON_GAMEOBJECT
#define COMMON_GAMEOBJECT

#include "Animator.h"
#include "Camera.h"
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
		Camera * m_camera;
		Renderer * m_renderer;

	};
};

#endif /* COMMON_GAMEOBJECT */