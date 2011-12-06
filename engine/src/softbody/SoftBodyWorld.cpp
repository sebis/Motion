#include "SoftBody.h"
#include "SoftBodyWorld.h"

namespace Common
{
	SoftBodyWorld::SoftBodyWorld()
	{
	}

	SoftBodyWorld::~SoftBodyWorld()
	{
	}

	void SoftBodyWorld::addSoftBody(SoftBody * body)
	{
		m_bodies.push_back(body);
	}

	void SoftBodyWorld::update(float dt)
	{
		float elapsed = dt / 1000.0f;

		int samples = 5;
		for (int i = 0; i < samples; i++)
		{
			for (Iterator it = m_bodies.begin(); it != m_bodies.end(); it++)
			{
				(*it)->update(elapsed / samples);
			}
		}
	}
}