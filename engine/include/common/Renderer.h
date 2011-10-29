#ifndef COMMON_RENDERER
#define COMMON_RENDERER

namespace Common
{
	class GameObject;

	class Renderer
	{
	public:
		Renderer(GameObject * gameObject) : m_gameObject(gameObject) {};
		virtual ~Renderer() {};

		virtual void draw() = 0;

	protected:
		GameObject * m_gameObject;
	};
};

#endif /* COMMON_RENDERER */