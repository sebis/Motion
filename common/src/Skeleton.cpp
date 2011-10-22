#include "Skeleton.h"

namespace Common
{
	void Skeleton::update(float dt)
	{
		computeIK();
	}

	void Skeleton::computeIK()
	{
		/*Bone * end = m_endEffectors[0];

		float x = end->transform().position().x;
		float y = end->transform().position().z;

		float L2 = glm::distance(end->parent()->transform().position(), end->transform().position());
		float L1 = glm::distance(end->parent()->parent()->transform().position(), end->parent()->transform().position());

		float oT = std::acos(x/std::sqrt(x*x+y*y));
		float o2 = std::acos((x*x + y*y - L1*L1 - L2*L2)/(2*L1*L2));
		float o1 = std::acos((L1*L1 + x*x + y*y - L2*L2)/(2*L1*std::sqrt(x*x + y*y))) + oT;

		end->parent()->parent()->transform().rotation() = glm::vec3(0.0f, o1 / 3.14159265 * 180.0, 0.0f);
		end->parent()->transform().rotation() = glm::vec3(0.0f, o2 / 3.14159265 * 180.0, 0.0f);*/
	}
}