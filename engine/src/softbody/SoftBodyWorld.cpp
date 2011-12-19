#include "CollisionDetector.h"
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

	void SoftBodyWorld::update(float /*dt*/)
	{
		//float elapsed = dt / 1000.0f;

		float fps = 1.0f/500.0f;
		for (Iterator it = m_bodies.begin(); it != m_bodies.end(); it++)
		{
			(*it)->update(fps);
		}
	}
}
