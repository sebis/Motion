#include "Transform.h"

#include "glm/gtc/type_precision.hpp"

namespace Common
{
	const glm::mat4 Transform::world() const
	{
		// TODO: this is now unnecessarily recomputed each time it is called.
		// Problem is that we don't know when position or rotation is updated
		// because we give out references to the variables.
		// Instead of updating the m_position reference externally maybe we
		// could give a pointer to a function so that the function can mark
		// the matrix dirty.
		glm::mat4 m(1.0f);

		if (m_parent)
			m *= m_parent->world();

		m = glm::translate(m, m_position);

		if (m_useQuaternions) {
			m *= glm::mat4_cast(glm::normalize(m_quaternion));
		} else {
			m = glm::rotate(m, m_rotation.x, glm::vec3(1, 0, 0));
			m = glm::rotate(m, m_rotation.y, glm::vec3(0, 1, 0));
			m = glm::rotate(m, m_rotation.z, glm::vec3(0, 0, 1));
		}

		m = glm::scale(m, m_scale);

		return m;
	}

	void Transform::extract(glm::mat4 m)
	{
		// Try to use as much precision as possible
		glm::highp_mat3 sub = glm::highp_mat3(glm::mat3(m));

		// calculate scale vector from column vector lengths
		glm::highp_vec3 scale(glm::length(sub[0]), glm::length(sub[1]), glm::length(sub[2]));

		// extract pure rotation matrix by dividing base vectors with scale
		glm::highp_mat3 r(sub[0] / scale.x, sub[1] / scale.y, sub[2] / scale.z);

		// extract matrix elements
		glm::highp_float r00 = r[0][0], r01 = r[1][0], r02 = r[2][0],
			r10 = r[0][1], r11 = r[1][1], r12 = r[2][1],
			r20 = r[0][2], r21 = r[1][2], r22 = r[2][2];

		// calculate rotation by (assume original order XYZ)
		glm::highp_float x = glm::atan(-r12,r22);
		glm::highp_float y = glm::asin(r02);
		glm::highp_float z = glm::atan(-r01,r00);

		// convert rotation to degrees
		glm::vec3 rotation(glm::highp_vec3(x, y, z) * glm::highp_float(180.0/3.14159265));

		// extract position vector
		glm::vec3 position(m[3]);

		// set computed values
		m_scale = scale;
		m_rotation = rotation;
		m_position = position;
	}
}