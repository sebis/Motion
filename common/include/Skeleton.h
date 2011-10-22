#ifndef COMMON_SKELETON
#define COMMON_SKELETON

#include "Bone.h"
#include "GameObject.h"

#include <vector>

namespace Common
{
	class Skeleton : public GameObject
	{
	public:
		Skeleton() {}
		virtual ~Skeleton() {}

		void draw() {}
		void update(float dt);

		inline void addEndEffector(Bone * bone) { m_endEffectors.push_back(bone); }

	private:
		void computeIK();

		std::vector<Bone *> m_endEffectors;
	};
}

#endif /* COMMON_SKELETON */