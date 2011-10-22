#include "GameObject.h"

namespace Common
{
	GameObject::GameObject()
		: m_animator(0), m_renderer(0)
	{
	}

	glm::mat4 GameObject::transformMatrix() const
	{
		return m_transform.world();
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
		delete m_renderer;
		delete m_animator;
	}
};