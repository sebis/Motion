#include "Bone.h"

namespace Common
{
	Bone::Bone(Bone * parent)
		: m_parent(parent)
	{
	}

	glm::mat4 Bone::absoluteTransform() const
	{
		if (!m_parent)
			return transform().world();

		return m_parent->absoluteTransform() * transform().world();
	}
}