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
		Bone(Transform& transform, Bone * parent = 0);
		virtual ~Bone() {}

		inline Transform& transform() { return m_transform; }
		inline const Transform& transform() const { return m_transform; }

		inline Bone * parent() const { return m_parent; }

		std::string name;

	private:
		Transform& m_transform;

		Bone * m_parent;
	};
}

#endif /* COMMON_BONE */