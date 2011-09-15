#include "GameObject.h"

namespace Common
{
	GameObject::GameObject()
		: m_animator(0), m_renderer(0)
	{
	}

	GameObject::~GameObject()
	{
		delete m_renderer;
		delete m_animator;
	}
};