#include "Collider.h"
#include "GameObject.h"

namespace Common
{
	Transform Collider::transform()
	{
		if (m_rigidBody)
			return m_rigidBody->transform();

		return m_gameObject->transform();
	}
}