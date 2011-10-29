#ifndef COMMON_BONE
#define COMMON_BONE

#include "MeshObject.h"
#include "Transform.h"

#include <string>
#include <vector>

namespace Common
{
	class RotationAxis
	{
	public:
		inline void setAxis(const glm::vec3& axis)
		{
			m_axis = axis;
		}

		inline void setConstraints(float min, float max)
		{
			m_min = min;
			m_max = max;
		}

		inline bool valid(float value)
		{
			return m_min <= value && value <= m_max;
		}

		inline bool free()
		{
			return glm::abs(m_max - m_min) > 0;
		}

		glm::vec3 m_axis;
		float m_min;
		float m_max;
		
	};

	class Bone
	{
	public:

		Bone(const std::string& name, Transform& transform, Bone * parent = 0);
		virtual ~Bone() {}

		inline Transform& transform() { return m_transform; }
		inline const Transform& transform() const { return m_transform; }

		inline Transform& localTransform() { return m_localTransform; }
		inline Bone * parent() const { return m_parent; }
		inline const std::string& name() const { return m_name; }

		inline RotationAxis * axes() { return m_axes; }
		inline RotationAxis * axis(int i) { return &m_axes[i]; }
		
	private:
		Transform& m_transform;
		RotationAxis m_axes[3];

		Transform m_localTransform;

		std::string m_name;
		Bone * m_parent;
	};
}

#endif /* COMMON_BONE */
