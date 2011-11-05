#include "GameObject.h"

namespace Common
{
	Camera * GameObject::s_camera = 0;

	GameObject::GameObject()
		: m_animator(0), m_renderer(0), m_rigidbody(0)
	{
	}

	void GameObject::update(float dt)
	{
		if (m_animator)
			m_animator->update(dt);
	}

	void GameObject::draw()
	{
		if (m_renderer)
			m_renderer->draw();
		if (m_animator)
			m_animator->visualize();
	}

	GameObject::~GameObject()
	{
		delete m_rigidbody;
		delete m_renderer;
		delete m_animator;
	}
};
