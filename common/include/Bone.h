#ifndef COMMON_BONE
#define COMMON_BONE

#include "MeshObject.h"
#include "Transform.h"

#include <string>

namespace Common
{
	class Bone
	{
	public:
		Bone(const std::string& name, Transform& transform, Bone * parent = 0);
		virtual ~Bone() {}

		inline Transform& transform() { return m_transform; }
		inline const Transform& transform() const { return m_transform; }

		inline Bone * parent() const { return m_parent; }
		inline const std::string& const name() { return m_name; }

		inline const glm::vec3& const axis() { return m_axis; }
		inline void setAxis(const glm::vec3& axis) { m_axis = axis; }

		Transform m_localTransform;

	private:
		Transform& m_transform;
		glm::vec3 m_axis;

		std::string m_name;
		Bone * m_parent;
	};
}

#endif /* COMMON_BONE */