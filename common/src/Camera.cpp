#include "Camera.h"
#include "Trace.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Common
{
	const float Camera::SPEED = 0.025f;
	const float Camera::TURN_SPEED = 30.0f;

	Camera::Camera(glm::vec3 position, glm::vec3 lookAt)
		: m_angles(0),
		  m_moving(0)
	{
		reset(position, lookAt);
	}

	Camera::~Camera()
	{
	}

	void Camera::reset(const glm::vec3& position, const glm::vec3& lookat)
	{
		m_position = position;
		m_lookDir = glm::normalize(lookat - position);

		m_axis = m_lookDir;
		m_angles = glm::vec3(0.0);
	}

	void Camera::update(float dt)
	{
		if (m_moving)
		{
			glm::vec3 move;

			// set current move vector
			if (m_moving & FORWARD)
				move += glm::vec3(0.0f, 0.0f, -dt);
			if (m_moving & BACKWARD)
				move += glm::vec3(0.0f, 0.0f, dt);
			if (m_moving & LEFT)
				move += glm::vec3(-dt, 0.0f, 0.0f);
			if (m_moving & RIGHT)
				move += glm::vec3(dt, 0.0f, 0.0f);

			move = glm::normalize(move) * SPEED;

			// Calculate camera position
			m_position += move * glm::mat3(m_view);
			Trace::info("Pos: (%f, %f, %f) -- Dir: (%f %f %f)\n", m_position.x, m_position.y, m_position.z, move.x, move.y, move.z);
		}

		m_view = glm::lookAt(m_position, m_position + m_lookDir, glm::vec3(0, 1, 0));
	}

	void Camera::turn(float h_amount, float v_amount)
	{
		// TODO: requires fixing
		m_angles.y += h_amount * TURN_SPEED;
		m_angles.x += v_amount * TURN_SPEED;

		glm::mat4 rotation;
		rotation = glm::rotate(rotation, m_angles.y, glm::vec3(0, 1, 0));
		rotation = glm::rotate(rotation, m_angles.x, glm::vec3(1, 0, 0));

		m_lookDir = m_axis * glm::mat3(rotation);
	}
}