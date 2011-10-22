#ifndef COMMON_BONE
#define COMMON_BONE

#include "Transform.h"

namespace Common
{
	class Bone
	{
	public:
		Bone(Bone * parent = 0);
		virtual ~Bone() {}

		inline Transform& transform() { return m_transform; }
		inline const Transform& transform() const { return m_transform; }
		glm::mat4 absoluteTransform() const;

	private:
		Transform m_transform;

		Bone * m_parent;
	};
}

#endif /* COMMON_BONE */