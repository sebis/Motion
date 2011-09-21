#ifndef COMMON_ANIMATOR
#define COMMON_ANIMATOR

namespace Common
{
	class GameObject;

	class Animator
	{
	public:
		Animator(GameObject * gameObject, bool loop = false) : m_gameObject(gameObject), m_loop(loop) {};
		virtual ~Animator() {};

		virtual void update(float dt) = 0;
		virtual void visualize() = 0;

	protected:
		GameObject * m_gameObject;
		bool m_loop;
	};
};

#endif /* COMMON_ANIMATOR */