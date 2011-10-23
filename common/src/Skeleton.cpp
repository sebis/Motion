#include "Skeleton.h"

namespace Common
{
	void Skeleton::update(float dt)
	{
		computeIK();
	}

	void Skeleton::computeIK()
	{
		for (int i = 0; i < m_endEffectors.size(); i++)
		{
			Bone * goal = m_endEffectors[i];
			Bone * joint = goal->parent();
			Bone * origin = joint->parent();
		
			float x = goal->transform().position().x;
			float y = -goal->transform().position().y;

			float L1 = 5.0f;//glm::distance(origin->transform().position(), joint->transform().position());
			float L2 = 5.0f;//glm::distance(joint->transform().position(), goal->transform().position());

			Trace::info("L: %f %f\n", L1, L2);

			float o2 = std::acos((x*x + y*y - L1*L1 - L2*L2)/(2*L1*L2));
			float n1 = (-x*(L2*std::sin(o2)) + y*(L1 + L2*std::cos(o2)));
			float n2 = (y*(L2*std::sin(o2)) + x*(L1 + L2*std::cos(o2)));
			float o1 = std::atan(n1/n2);

			// TODO: 90 degress offset? why?
			origin->transform().rotation() = glm::vec3(0.0f, 0.0f, 90.0f + (-o1 / 3.14159265 * 180.0));
			joint->transform().rotation() = glm::vec3(0.0f, 0.0f, -o2 / 3.14159265 * 180.0);
		}
	}
}