#ifndef COMMON_FLOWPRIMITIVE
#define COMMON_FLOWPRIMITIVE

#include "Trace.h"

#include "glm/glm.hpp"

namespace Common
{
	class FlowPrimitive
	{
	public:
		FlowPrimitive() {}
		virtual ~FlowPrimitive() {}
		
		virtual glm::vec3 velocityAt(const glm::vec3 & position) = 0;
	};

	class UniformFlow : public FlowPrimitive
	{
	public:
		UniformFlow(const glm::vec3 & velocity, const glm::vec3 & center = glm::vec3(0.0f), float radius = +1e10)
			: m_center(center), m_velocity(velocity), m_radius(radius) {}
		virtual ~UniformFlow() {}

		glm::vec3 velocityAt(const glm::vec3 & position);

	private:
		glm::vec3 m_center;
		glm::vec3 m_velocity;
		float m_radius;
	};

	class SourceFlow : public FlowPrimitive
	{
	public:
		SourceFlow(const glm::vec3 & center, float radius, float magnitude)
			: m_center(center), m_radius(radius), m_magnitude(magnitude) {}
		virtual ~SourceFlow() {}

		glm::vec3 velocityAt(const glm::vec3 & position);

	private:
		glm::vec3 m_center;
		float m_radius;
		float m_magnitude;
	};

	class VortexFlow : public FlowPrimitive
	{
	public:
		VortexFlow(const glm::vec3 & center, const glm::vec3 & axis, float radius, float magnitude)
			: m_center(center), m_axis(axis), m_radius(radius), m_magnitude(magnitude) {}
		virtual ~VortexFlow() {}

		glm::vec3 velocityAt(const glm::vec3 & position);

	private:
		glm::vec3 m_center;
		glm::vec3 m_axis;
		float m_radius;
		float m_magnitude;
	};
}

#endif /* COMMON_FLOWPRIMITIVE */