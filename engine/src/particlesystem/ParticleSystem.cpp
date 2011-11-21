#include "ParticleSystem.h"
#include "Utils.h"

namespace
{
	Utils::Random s_random;
}

namespace Common
{
	ParticleSystem::ParticleSystem(const ParticleSettings & settings)
		: m_settings(settings),
		firstActiveParticle(0),
		firstFreeParticle(0)
	{
		m_particles = new Particle[m_settings.maxParticles];
		m_renderer = new ParticleRenderer(new Texture(m_settings.texture.c_str()));
		m_renderer->setBlendMode(settings.srcBlend, settings.dstBlend);

		//m_flows.push_back(new UniformFlow(glm::vec3(0.0f, 2.0f, 0.0f)));
		//m_flows.push_back(new SourceFlow(glm::vec3(4.0f, 0.0f, 0.0f), 2.0f, -10.0f));
		//m_flows.push_back(new VortexFlow(glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 5.0f, 2.0f));
		//m_flows.push_back(new VortexFlow(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 5.0f, 2.0f));
	}

	ParticleSystem::~ParticleSystem()
	{
		delete [] m_particles;
		delete m_renderer;
	}

	void ParticleSystem::update(float dt)
	{
		// convert to seconds
		float elapsed = dt / 1000.0f;

		unsigned currentParticle = firstActiveParticle;

		while (currentParticle != firstFreeParticle)
		{
			Particle * p = &m_particles[currentParticle];

			if (p->age >= m_settings.duration) {
				firstActiveParticle++;
				if (firstActiveParticle >= m_settings.maxParticles)
					firstActiveParticle = 0;
			}
			else {
				p->age += elapsed * (1 + p->ageRandom);

				glm::vec3 force;

				for (FlowIterator it = m_flows.begin(); it != m_flows.end(); ++it)
				{
					FlowPrimitive * flow = *it;

					glm::vec3 vr = flow->velocityAt(p->position);
					force += 1.0f * vr;
				}
				
				//p->velocity += m_settings.gravity * elapsed;
				p->velocity += force * elapsed;
				p->position += p->velocity * elapsed;

				float t = p->age / m_settings.duration;

				p->size = glm::mix(p->startSize, p->endSize, t);
				p->color = glm::mix(m_settings.minColor, m_settings.maxColor, p->colorRandom);
				p->color.a *= 1-t;
			}

			currentParticle++;
			if (currentParticle >= m_settings.maxParticles)
				currentParticle = 0;
		}
	}

	void ParticleSystem::draw()
	{
		// nothing to draw
		if (firstActiveParticle == firstFreeParticle)
			return;

		// check if we need to issue two drawing calls (in case the buffer has wrapped)
		if (firstActiveParticle < firstFreeParticle) {
			m_renderer->draw(&m_particles[firstActiveParticle], firstFreeParticle - firstActiveParticle);
		}
		else {
			m_renderer->draw(&m_particles[firstActiveParticle], m_settings.maxParticles - firstActiveParticle);
			m_renderer->draw(&m_particles[0], firstFreeParticle);
		}
	}

	void ParticleSystem::addParticle(const glm::vec3 & position, const glm::vec3 & velocity)
	{
		unsigned nextFreeParticle = firstFreeParticle + 1;

		if (nextFreeParticle >= m_settings.maxParticles)
			nextFreeParticle = 0;

		if (nextFreeParticle == firstActiveParticle)
			return;

		Particle p;

		p.age = 0;
		p.size = 1.0f;
		p.position = position;

		p.velocity = velocity;
		p.velocity += glm::vec3(
			glm::mix(m_settings.minVelocity.x, m_settings.maxVelocity.x, s_random.rand01()),
			glm::mix(m_settings.minVelocity.y, m_settings.maxVelocity.y, s_random.rand01()),
			glm::mix(m_settings.minVelocity.z, m_settings.maxVelocity.z, s_random.rand01()));

		p.color = glm::mix(m_settings.minColor, m_settings.maxColor, s_random.rand01());

		p.startSize = glm::mix(m_settings.minStartSize, m_settings.maxStartSize, s_random.rand01());
		p.endSize = glm::mix(m_settings.minEndSize, m_settings.maxEndSize, s_random.rand01());

		p.colorRandom = (float)s_random.rand01();
		p.ageRandom = (float)s_random.rand01();

		m_particles[firstFreeParticle] = p;

		firstFreeParticle = nextFreeParticle;
	}

	void ParticleSystem::addFlow(FlowPrimitive * flow)
	{
		m_flows.push_back(flow);
	}
}
