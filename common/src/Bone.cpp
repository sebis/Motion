#include "Bone.h"

namespace Common
{
	Bone::Bone(Transform& transform, Bone * parent)
		: m_transform(transform), m_parent(parent)
	{
	}
}