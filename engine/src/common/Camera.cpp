#include "Camera.h"
#include "Trace.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace Common
{
	float Camera::s_aspect = 0.0f;

	const float Camera::MOVE_SPEED = 0.25f;
	const float Camera::TURN_SPEED = 0.15f;

	Camera::Camera(glm::vec3 position, glm::vec3 lookAt)
		: m_up(0, 1, 0),
		  m_moving(0)
	{
		reset(position, lookAt);
	}

	Camera::~Camera()
	{
	}

	void Camera::reset(const glm::vec3& position, const glm::vec3& lookAt)
	{
		m_position = position;
		m_lookDir = glm::normalize(lookAt - position);

		m_view = glm::lookAt(m_position, m_position + m_lookDir, m_up);
	}

	void Camera::update(float dt)
	{
		if (m_moving)
		{
			glm::vec3 move;

			// set current move vector
			if (m_moving & FORWARD)
				move += glm::vec3(0.0f, 0.0f, -dt);
			else if (m_moving & BACKWARD)
				move += glm::vec3(0.0f, 0.0f, dt);

			if (m_moving & LEFT)
				move += glm::vec3(-dt, 0.0f, 0.0f);
			else if (m_moving & RIGHT)
				move += glm::vec3(dt, 0.0f, 0.0f);

			move = glm::normalize(move) * MOVE_SPEED;

			// Calculate new position with rotated move direction
			m_position += move * glm::mat3(m_view);

			// update view matrix
			m_view = glm::lookAt(m_position, m_position + m_lookDir, m_up);
		}
	}

	void Camera::turn(glm::vec2 mouseDelta)
	{
		mouseDelta *= TURN_SPEED;

		// right vector
		glm::vec3 axis = glm::cross(m_lookDir, m_up);

		// represent lookdir as a position in space we want to rotate
		glm::quat lookdir(0, m_lookDir);

		// construct quaternions that will rotate lookdir around specified axes
		glm::quat q = glm::angleAxis(mouseDelta.y, axis);
		glm::quat p = glm::angleAxis(mouseDelta.x, m_up);

		// rotate lookdir by q and p
		lookdir = (q * p) * lookdir * glm::conjugate(q * p);

		// extract the rotated vector
		m_lookDir = glm::vec3(lookdir.x, lookdir.y, lookdir.z);

		// update view matrix
		m_view = glm::lookAt(m_position, m_position + m_lookDir, m_up);
	}
}
