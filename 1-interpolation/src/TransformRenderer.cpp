#include "GameObject.h"
#include "TransformRenderer.h"

#include "glm/gtc/quaternion.hpp"

namespace Interpolation
{
	TransformRenderer::TransformRenderer(Common::GameObject * gameObject, Shader * shader, const Common::Transform* transform)
		: Renderer(gameObject), m_shader(shader), m_transform(transform)
	{
		// TODO: don't load separately
		m_pitch = Common::MeshFactory::FromFile("resources/gyro.ply", glm::vec4(1, 0, 0, 1)); // around X
		m_yaw = Common::MeshFactory::FromFile("resources/gyro.ply", glm::vec4(0, 1, 0, 1)); // around Y
		m_roll = Common::MeshFactory::FromFile("resources/gyro.ply", glm::vec4(0, 0, 1, 1)); // around Z
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
		
		// create separate matrices so that they can be combined to create a "proper" gyroscope
		glm::mat4 mx = glm::rotate(glm::mat4(1.0f), m_transform->rotation().x, glm::vec3(1, 0, 0));
		glm::mat4 my = glm::rotate(glm::mat4(1.0f), m_transform->rotation().y, glm::vec3(0, 1, 0));
		glm::mat4 mz = glm::rotate(glm::mat4(1.0f), m_transform->rotation().z, glm::vec3(0, 0, 1));
		
		// mesh is oriented to Z-axis so rotate for correct axis
		m_shader->setUniform("world", m_gameObject->m_transform.world() * glm::rotate(mx, 90.0f, glm::vec3(0, 1, 0)));
		m_pitch->draw();		

		// mesh is oriented to Z-axis so rotate for correct axis
		m_shader->setUniform("world", m_gameObject->m_transform.world() * glm::rotate(mx * my, 90.0f, glm::vec3(1, 0, 0)));
		m_yaw->draw();

		m_shader->setUniform("world", m_gameObject->m_transform.world() * mx * my * mz);
		m_roll->draw();

		m_shader->unbind();

		glEnable(GL_BLEND);
	}
}