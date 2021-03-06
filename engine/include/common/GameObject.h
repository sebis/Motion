#ifndef COMMON_GAMEOBJECT
#define COMMON_GAMEOBJECT

#include "Animator.h"
#include "Camera.h"
#include "Component.h"
#include "Transform.h"
#include "Renderer.h"
#include "RigidBody.h"

namespace Common
{

	class GameObject : public Component
	{
	public:
		GameObject();
		virtual ~GameObject();

		virtual void draw();
		virtual void update(float dt);

		inline Transform& transform() { return m_transform; }

		Animator * m_animator;
		Renderer * m_renderer;
		RigidBody * m_rigidbody;

		// TODO: smarted way to set camera
		static Camera * s_camera;

	private:
		Transform m_transform;
	};
};

#endif /* COMMON_GAMEOBJECT */
