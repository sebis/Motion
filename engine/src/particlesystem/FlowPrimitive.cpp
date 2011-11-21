#include "FlowPrimitive.h"

namespace Common
{
	glm::vec3 UniformFlow::velocityAt(const glm::vec3 & position)
	{
		glm::vec3 r = position - m_center;
		float d = glm::length(r);
		float r0 = m_radius;

		if (d <= m_radius)
			return m_velocity;

		return glm::vec3(0.0f);
	}

	glm::vec3 SourceFlow::velocityAt(const glm::vec3 & position)
	{
		glm::vec3 r = position - m_center;
		float d = glm::length(r);
		float r0 = m_radius;
		float m = m_magnitude;

		if (d <= r0)
			return (1 - d / r0) * glm::normalize(r) * m;

		return glm::vec3(0.0f);
	}

	glm::vec3 VortexFlow::velocityAt(const glm::vec3 & position)
	{
		glm::vec3 r = position - m_center;
		float d = glm::length(r);
		float r0 = m_radius;
		float m = m_magnitude;

		if (d <= r0)
			return (1 - d / r0) * glm::normalize(glm::cross(r, m_axis)) * m;

		return glm::vec3(0.0f);
	}
}