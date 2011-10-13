#include "Camera.h"
#include "Trace.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace Common
{
	const float Camera::SPEED = 0.025f;
	const float Camera::TURN_SPEED = 80.0f;

	Camera::Camera(glm::vec3 position, glm::vec3 lookAt)
		: m_moving(0),
		  m_rotation(0),
		  m_up(0, 1, 0)
	{
		reset(position, lookAt);
	}

	Camera::~Camera()
	{
	}

	void Camera::reset(const glm::vec3& position, const glm::vec3& lookat)
	{
		m_position = position;
		m_lookAt = lookat;
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
			Trace::info("Pos: (%f, %f, %f)\n", m_position.x, m_position.y, m_position.z);
		}

		m_view = glm::lookAt(m_position, m_lookAt, m_up);
	}

	void Camera::turn(int h_amount, int v_amount)
	{
		float mouseDirectionX = h_amount/16.0f;
		float mouseDirectionY = v_amount/16.0f;
		// TODO: requires fixing
		//m_rotationX += mouseDirectionY;
		/*if (m_rotationX < -180.0f || m_rotationX > 180.0f)
		{
			m_rotationX = glm::clamp(m_rotationX, -180.0f, 180.0f);
			return;
		}*/

		//Trace::info("Turning: %f %f\n", mouseDirectionX, mouseDirectionY);
		Trace::info("Look dir: %f %f %f\n", m_lookAt.x, m_lookAt.y, m_lookAt.z);
		

		glm::vec3 axis = glm::cross(glm::normalize(m_lookAt - m_position), m_up);
		//rotateAroundAxis(mouseDirectionY, axis);
		//rotateAroundAxis(mouseDirectionX, glm::vec3(0, 1, 0));

		glm::quat lookat(0, m_lookAt - m_position);

		glm::quat q = glm::normalize(glm::angleAxis(mouseDirectionY, axis));
		glm::quat q2 = glm::normalize(glm::angleAxis(mouseDirectionX, m_up));
		lookat = (q2 * q) * lookat * glm::conjugate(q2 * q);

		m_lookAt = m_position + glm::vec3(lookat.x, lookat.y, lookat.z);
	}

	void Camera::rotateAroundAxis(float angle, glm::vec3 axis)
	{
		

		
	}
}