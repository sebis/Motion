#include "Collider.h"
#include "GameObject.h"
#include "RigidBody.h"

namespace Common
{
	Transform Collider::transform() const
	{
		RigidBody * rigidBody = dynamic_cast<RigidBody *>(m_collisionBody);
		if (rigidBody)
			return rigidBody->transform();

		return m_gameObject->transform();
	}
}