#include "ParticleSystem.h"
#include "Utils.h"

namespace
{
	Utils::Random s_random;
}

namespace Common
{
	ParticleSystem::ParticleSystem()
	{
		m_renderer = new ParticleRenderer(Shader::find("point"), new Texture("resources/fire_paint.bmp"), m_particles);

		m_settings = new ParticleSettings();

		m_settings->gravity = glm::vec3(0.0f, 1.0f, 0.0f);
		m_settings->duration = 2.0f;

		m_settings->minVelocity = glm::vec3(-1.0f, 0.0f, -1.0f);
		m_settings->maxVelocity = glm::vec3(1.0f);
	}

	ParticleSystem::~ParticleSystem()
	{
		delete m_settings;
	}

	void ParticleSystem::update(float dt)
	{
		// convert to seconds
		float elapsed = dt / 1000.0;

		for (ParticleIterator it = m_particles.begin(); it != m_particles.end();)
		{
			if (it->age >= m_settings->duration) {
				it = m_particles.erase(it);
			}
			else {
				it->age += elapsed;

				it->velocity += m_settings->gravity * elapsed;
				it->position += it->velocity * elapsed;

				it++;
			}
		}
	}

	void ParticleSystem::draw()
	{
		m_renderer->draw();
	}

	void ParticleSystem::addParticle(const glm::vec3 & position, const glm::vec3 & velocity)
	{
		if (m_particles.size() >= 200)
			return;

		Particle p;
		p.position = position;
		p.age = 0;

		p.velocity = velocity;
		p.velocity += glm::vec3(
			glm::mix(m_settings->minVelocity.x, m_settings->maxVelocity.x, s_random.rand01()),
			glm::mix(m_settings->minVelocity.y, m_settings->maxVelocity.y, s_random.rand01()),
			glm::mix(m_settings->minVelocity.z, m_settings->maxVelocity.z, s_random.rand01()));

		m_particles.push_back(p);
	}
}