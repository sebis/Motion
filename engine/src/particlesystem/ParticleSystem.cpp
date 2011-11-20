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
		m_renderer = new ParticleRenderer(Shader::find("point"), new Texture("resources/fire.bmp"), m_particles);

		m_settings = new ParticleSettings();

		m_settings->maxParticles = 5000;
		m_settings->gravity = glm::vec3(0.0f, 1.0f, 0.0f);

		m_settings->minDuration = 0.5f;
		m_settings->maxDuration = 1.5f;

		m_settings->minVelocity = glm::vec3(-1.0f, 0.0f, -1.0f);
		m_settings->maxVelocity = glm::vec3(1.0f);

		m_settings->minStartSize = 0.5f;
		m_settings->maxStartSize = 1.0f;

		m_settings->minEndSize = 2.0f;
		m_settings->maxEndSize = 4.0f;

		m_settings->minColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_settings->maxColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		m_settings->minStartAlpha = 0.50f;
		m_settings->maxStartAlpha = 0.75f;
	}

	ParticleSystem::~ParticleSystem()
	{
		delete m_settings;
	}

	void ParticleSystem::update(float dt)
	{
		//Trace::info("Particles %d\n", m_particles.size());
		// convert to seconds
		float elapsed = dt / 1000.0;

		for (ParticleIterator it = m_particles.begin(); it != m_particles.end();)
		{
			if (it->age >= it->duration) {
				it = m_particles.erase(it);
			}
			else {
				it->age += elapsed;

				it->velocity += m_settings->gravity * elapsed;
				it->position += it->velocity * elapsed;

				float t = it->age / it->duration;

				it->size = glm::mix(it->startSize, it->endSize, t);
				it->color.a = glm::mix(it->startAlpha, 0.0f, t);

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
		if (m_particles.size() >= m_settings->maxParticles)
			return;

		Particle p;
		p.position = position;

		p.age = 0;
		p.duration = glm::mix(m_settings->minDuration, m_settings->maxDuration, s_random.rand01());

		p.velocity = velocity;
		p.velocity += glm::vec3(
			glm::mix(m_settings->minVelocity.x, m_settings->maxVelocity.x, s_random.rand01()),
			glm::mix(m_settings->minVelocity.y, m_settings->maxVelocity.y, s_random.rand01()),
			glm::mix(m_settings->minVelocity.z, m_settings->maxVelocity.z, s_random.rand01()));

		p.color = glm::mix(m_settings->minColor, m_settings->maxColor, s_random.rand01());

		p.startSize = glm::mix(m_settings->minStartSize, m_settings->maxStartSize, s_random.rand01());
		p.endSize = glm::mix(m_settings->minEndSize, m_settings->maxEndSize, s_random.rand01());

		p.startAlpha = glm::mix(m_settings->minStartAlpha, m_settings->maxStartAlpha, s_random.rand01());

		m_particles.push_back(p);
	}
}