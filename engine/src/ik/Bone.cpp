#include "Bone.h"

namespace Common
{
	Bone::Bone(const std::string& name, Transform& transform, Bone * parent)
		: m_name(name), m_transform(transform), m_parent(parent)
	{
		// Initialize values
		m_axes[0].setAxis(glm::vec3(1.0f, 0.0f, 0.0f));
		m_axes[0].setConstraints(0.0f, 0.0f);
		m_axes[1].setAxis(glm::vec3(0.0f, 1.0f, 0.0f));
		m_axes[1].setConstraints(0.0f, 0.0f);
		m_axes[2].setAxis(glm::vec3(0.0f, 0.0f, 1.0f));
		m_axes[2].setConstraints(0.0f, 0.0f);
	}
}