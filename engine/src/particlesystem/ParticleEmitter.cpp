#include "ParticleEmitter.h"
#include "Utils.h"

namespace Common
{
	namespace
	{
		Utils::Random r;
	}

	void CircleParticleEmitter::update(float dt)
	{
		for (int i = 0; i < available(dt); i++)
		{
			float angle = float(r.rand01() * 2 * M_PI);
			glm::vec3 position(m_radius * glm::cos(angle), 0.0f, m_radius * glm::sin(angle));

			m_system->addParticle(position, glm::vec3(0.0f));
		}
	}

	void SphereParticleEmitter::update(float dt)
	{
		for (int i = 0; i < available(dt); i++)
		{
			float omega = float(r.rand01() * M_PI);
			float phi = float(r.rand01() * 2 * M_PI);

			float r = radius();

			float x = r * glm::sin(omega) * glm::cos(phi);
			float y = r * glm::sin(omega) * glm::sin(phi);
			float z = r * glm::cos(omega);

			m_system->addParticle(glm::vec3(x, y, z), glm::vec3(0.0f));
		}
	}

	void TrailParticleEmitter::update(float dt)
	{
		if (m_positions.empty())
			return;

		for (int i = 0; i < available(dt); i++)
		{
			int index = int(r.rand01() * m_positions.size());
			if (index == m_positions.size())
				continue;

			m_system->addParticle(m_positions[index], glm::vec3(0.0f));
		}
	}
}