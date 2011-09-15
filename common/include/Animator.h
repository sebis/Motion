#ifndef COMMON_ANIMATOR
#define COMMON_ANIMATOR

namespace Common
{
	class GameObject;

	class Animator
	{
	public:
		Animator(GameObject * gameObject) : m_gameObject(gameObject) {};
		virtual ~Animator() {};

		virtual void update(float dt) = 0;

	protected:
		GameObject * m_gameObject;
	};
};

#endif /* COMMON_ANIMATOR */