#include "Bone.h"

namespace Common
{
	Bone::Bone(const std::string& name, Transform& transform, Bone * parent)
		: m_name(name), m_transform(transform), m_parent(parent)
	{
	}
}