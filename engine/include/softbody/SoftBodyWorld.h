#ifndef COMMON_SOFTBODYWORLD
#define COMMON_SOFTBODYWORLD

#include <vector>

namespace Common
{
	class SoftBody;
	struct CollisionData;

	class SoftBodyWorld
	{
	public:
		SoftBodyWorld();
		virtual ~SoftBodyWorld();

		void addSoftBody(SoftBody * softBody);
		void update(float dt);

	private:
		void resolveInterpenetration(const CollisionData & data);

		typedef std::vector<SoftBody *> Container;
		typedef Container::iterator Iterator;
		Container m_bodies;
	};
}

#endif /* COMMON_SOFTBODYWORLD */