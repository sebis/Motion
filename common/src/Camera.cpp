#include "Camera.h"
#include "Trace.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Common
{
	const float Camera::SPEED = 0.025f;
	const float Camera::TURN_SPEED = 30.0f;

	Camera::Camera()
		: m_position(10.0f, 10.0f, 10.0f),
		  m_angles(0),
		  m_lookDir(-1.0f, -1.0f, -1.0f),
		  m_moving(0)
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::update(float dt)
	{
		glm::mat4 rotation;
		rotation = glm::rotate(rotation, 0.0f, glm::vec3(0, 0, -1));
		rotation = glm::rotate(rotation, -135.0f, glm::vec3(0, 1, 0));
		rotation = glm::rotate(rotation, 0.0f, glm::vec3(1, 0, 0));

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

		glm::vec3 dir = glm::normalize(glm::mat3(rotation) * glm::vec3(0, 0, -1));
		

		// Transform camera
		glm::mat4 view(1.0f);
		glm::rotate(view, 135.0f, glm::vec3(0, 1, 0));
		glm::rotate(view, 0.0f, glm::vec3(1, 0, 0));
		//m_view = view * glm::translate(glm::mat4(1.0f), -m_position);
		m_view = glm::lookAt(m_position, m_position + m_lookDir, glm::vec3(0, 1, 0));
	}

	void Camera::turn(float h_amount, float v_amount)
	{
		Trace::info("m_angles.y: %f\n", m_angles.y);
		m_angles.y += h_amount * TURN_SPEED;
		m_angles.x += v_amount * TURN_SPEED;
		glm::mat4 rotation;
		rotation = glm::rotate(rotation, m_angles.y, glm::vec3(0, 1, 0));
		rotation = glm::rotate(rotation, m_angles.x, glm::vec3(1, 0, 0));
		m_lookDir = glm::vec3(-1.0f) * glm::mat3(rotation);
	}
}