#include "GameObject.h"
#include "TransformRenderer.h"

#include "glm/gtc/quaternion.hpp"

namespace Interpolation
{
	TransformRenderer::TransformRenderer(Common::GameObject * gameObject, Shader * shader, const Common::Transform* transform)
		: Renderer(gameObject), m_shader(shader), m_transform(transform)
	{
		m_pitch = Common::MeshFactory::FromFile("resources/gyrox.ply", glm::vec4(1, 0, 0, 1)); // around X
		m_yaw = Common::MeshFactory::FromFile("resources/gyroy.ply", glm::vec4(0, 1, 0, 1)); // around Y
		m_roll = Common::MeshFactory::FromFile("resources/gyroz.ply", glm::vec4(0, 0, 1, 1)); // around Z
	}

	TransformRenderer::~TransformRenderer()
	{
		delete m_yaw;
		delete m_roll;
		delete m_pitch;
	}

	void TransformRenderer::draw()
	{
		// TODO: handle render state properly
		glDisable(GL_BLEND);

		m_shader->bind();

		m_shader->setUniform("view", m_gameObject->m_camera->view());
		m_shader->setUniform("projection", m_gameObject->m_camera->projection());

		//glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), m_transform->rotation(), glm::vec3(1, 0, 0));
		
		glm::mat4 mx(1.0f);
		//mx = glm::rotate(mx, m_transform->rotation().z, glm::vec3(0, 0, 1));
		//mx = glm::rotate(mx, m_transform->rotation().y, glm::vec3(0, 1, 0));
		mx = glm::rotate(mx, m_transform->rotation().x, glm::vec3(1, 0, 0));
		
		m_shader->setUniform("world", m_gameObject->m_transform.world() * mx);
		m_pitch->draw();

		glm::mat4 my = glm::mat4(1.0f);
		//my = glm::rotate(my, m_transform->rotation().z, glm::vec3(0, 0, 1));
		my = glm::rotate(my, m_transform->rotation().y, glm::vec3(0, 1, 0));
		//glm::mat4 tmp = glm::rotate(glm::mat4(), m_transform->rotation().x, glm::vec3(1, 0, 0));
		m_shader->setUniform("world", m_gameObject->m_transform.world() * mx * my * glm::inverse(mx));
		m_yaw->draw();

		glm::mat4 mz = glm::rotate(glm::mat4(1.0f), m_transform->rotation().z, glm::vec3(0, 0, 1));
		//mz = glm::inverse(my) * glm::inverse(mx) * mz * mx * my;

		m_shader->setUniform("world", m_gameObject->m_transform.world() * (mx * my) * mz * glm::inverse(mx * my));
		m_roll->draw();

		m_shader->unbind();

		glEnable(GL_BLEND);
	}
}