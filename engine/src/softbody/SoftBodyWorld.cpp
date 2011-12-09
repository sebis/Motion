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

	void SoftBodyWorld::update(float dt)
	{
		float elapsed = dt / 1000.0f;

		int samples = 1;
		float fps = 1.0f/1000.0f;
		for (int i = 0; i < samples; i++)
		{
			for (Iterator it = m_bodies.begin(); it != m_bodies.end(); it++)
			{
				(*it)->update(fps);
			}
		}

		// 3. Contact generator (generate collision data for next step)
		/*std::vector<CollisionData> collisions;
		CollisionDetector::instance()->detectCollisions(collisions);

		// 4. Resolve collisions and write velocity to rigidbody data
		for (unsigned i = 0; i < collisions.size(); i++)
		{
			const CollisionData & data = collisions[i];

			resolveInterpenetration(data);
		}*/
	}

	void SoftBodyWorld::resolveInterpenetration(const CollisionData & data)
	{
		for (int i = 0; i < data.contacts.size(); i++)
		{
			Contact * contact = data.contacts[i];

			glm::vec3 * vertex = static_cast<glm::vec3 *>(contact->userData);
			*vertex += contact->penetration * contact->normal;
			Trace::info("Resolving: %f\n", contact->penetration);
		}
	}
}