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
			: m_position(glm::vec3(0.0f)),
			  m_rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
			m_transform(glm::mat4(1.0f))
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
			// TODO: this is now unnecessarily recomputed each time it is called.
			// Problem is that we don't know when position or rotation is updated
			// because we give out references to the variables.
			// Instead of updating the m_position reference externally maybe we
			// could give a pointer to a function so that the function can mark
			// the matrix dirty.
			glm::mat4 m(1.0f);
			m = glm::translate(m, m_position);
			m = m * glm::mat4(m_rot);
			/*m = glm::rotate(m, m_rotation.x, glm::vec3(1, 0, 0));
			m = glm::rotate(m, m_rotation.y, glm::vec3(0, 1, 0));
			m = glm::rotate(m, m_rotation.z, glm::vec3(0, 0, 1));*/
			return m;
		}

		//inline const glm::vec3& position() const { return m_position; }
		inline glm::vec3& position() { return m_position; }

		inline glm::vec3& rotation() { return m_rotation; }
		inline glm::mat3& rot() { return m_rot; }

		inline void reset()
		{
			m_position = glm::vec3(0.0f);
			//m_rotation = glm::vec3(0.0f);
		}

	private:
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;

		glm::mat4 m_transform;

		glm::mat3 m_rot;
	};
}

#endif /* COMMON_TRANSFORM */