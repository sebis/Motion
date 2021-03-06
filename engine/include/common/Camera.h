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

		Camera(glm::vec3 position, glm::vec3 lookAt = glm::vec3(0.0f));
		virtual ~Camera();

		inline void raiseFlag(int flag) { m_moving |= flag; }
		inline void dropFlag(int flag) { m_moving &= ~flag; }

		inline const glm::mat4& view() const { return m_view; }
		inline const glm::mat4& projection() const { return m_projection; }

		void reset(const glm::vec3& position, const glm::vec3& lookat = glm::vec3(0.0f));

		inline void set_projection(const glm::mat4& projection) { m_projection = projection; }

		void update(float dt);

		void turn(glm::vec2 mouseDelta);

		// TODO: better access to aspect ratio
		static float s_aspect;

	private:
		static const float MOVE_SPEED;
		static const float TURN_SPEED;

		glm::mat4 m_projection;
		glm::mat4 m_view;

		glm::vec3 m_position;
		glm::vec3 m_lookDir;
		glm::vec3 m_up;

		// bit field for moving
		int m_moving;
	};
}

#endif /* COMMON_CAMERA */
