#ifndef COMMON_CAMERA
#define COMMON_CAMERA

#include "glm/glm.hpp"

namespace Common
{
	class Camera
	{
	public:
		enum {
			FORWARD  = (1 << 0),
			BACKWARD = (1 << 1),
			LEFT     = (1 << 2),
			RIGHT    = (1 << 3),
		};

		Camera();
		virtual ~Camera();

		inline void raiseFlag(int flag) { m_moving |= flag; }
		inline void dropFlag(int flag) { m_moving &= ~flag; }

		inline glm::vec3& angles() { return m_angles; }
		inline const glm::mat4& view() const { return m_view; }
		inline const glm::mat4& projection() const { return m_projection; }

		inline void set_projection(const glm::mat4& projection) { m_projection = projection; }

		void update(float dt);

		void turn(float h_amount, float v_amount);

	private:
		static const float SPEED;
		static const float TURN_SPEED;

		glm::mat4 m_projection;
		glm::mat4 m_view;

		glm::vec3 m_position;
		glm::vec3 m_lookDir;
		glm::vec3 m_angles;

		// bit field for moving
		int m_moving;
	};
}

#endif /* COMMON_CAMERA */