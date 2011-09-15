#ifndef COMMON_TRANSFORM
#define COMMON_TRANSFORM

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Common
{
	class Transform
	{
	public:
		Transform()
			: m_position(glm::vec3(0.0f)), m_transform(glm::mat4(1.0f))
		{}

		static Transform Translation(const float& x, const float& y, const float& z)
		{
			return Translation(glm::vec3(x, y, z));
		}

		static Transform Translation(const glm::vec3& translate)
		{
			Transform t;
			t.translate(translate);
			return t;
		}

		inline void translate(const glm::vec3& translate)
		{
			m_position += translate;
		}

		inline const glm::mat4 world() const
		{
			return glm::translate(glm::mat4(1.0f), m_position);
		}

		inline const glm::vec3& position() const
		{
			return m_position;
		}

		inline void reset()
		{
			m_position = glm::vec3(0.0f);
		}

		glm::vec3 m_position;
	private:
		
		glm::vec3 m_rotation;
		glm::vec3 m_scale;

		glm::mat4 m_transform;
	};
}

#endif /* COMMON_TRANSFORM */