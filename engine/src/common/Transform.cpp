/**
 * Copyright 2011-2012 Sebastian Eriksson.
 * This file is part of Motion.
 *
 * Motion is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Motion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Motion. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Transform.h"

#include "glm/gtc/type_precision.hpp"

#include <cmath>

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

		if (m_rotationType == MATRIX) {
			m *= glm::mat4(m_rotationMatrix);
		} else if (m_rotationType == QUATERNION) {
			m *= glm::mat4_cast(glm::normalize(m_quaternion));
		} else {
			m = glm::rotate(m, m_rotation.x, glm::vec3(1, 0, 0));
			m = glm::rotate(m, m_rotation.y, glm::vec3(0, 1, 0));
			m = glm::rotate(m, m_rotation.z, glm::vec3(0, 0, 1));
		}
		
		m = glm::scale(m, m_scale);

		return m;
	}

	Transform Transform::extract(const glm::mat4& m, RotationType rotationType)
	{
		Transform t;

		// Try to use as much precision as possible
		glm::highp_mat3 sub = glm::highp_mat3(glm::mat3(m));

		// extract position vector
		t.m_position = glm::vec3(m[3]);

		// calculate scale vector from column vector lengths
		glm::highp_vec3 scale(glm::length(sub[0]), glm::length(sub[1]), glm::length(sub[2]));

		// extract scale vector
		t.m_scale = scale;

		// extract pure rotation matrix by dividing base vectors with scale
		glm::highp_mat3 r(sub[0] / scale.x, sub[1] / scale.y, sub[2] / scale.z);

		if (rotationType == MATRIX) {
			t.m_rotationMatrix = r;
			return t;
		}

		if (rotationType == QUATERNION) {
			t.m_quaternion = glm::quat_cast(glm::mat3(r));
			return t;
		}

		// calculate rotation by (assume original order XYZ)
		glm::highp_float x = glm::atan(-r[2][1],r[2][2]);
		glm::highp_float y = glm::asin(r[2][0]);
		glm::highp_float z = glm::atan(-r[1][0],r[0][0]);

		// convert rotation to degrees
		glm::vec3 rotation(glm::highp_vec3(x, y, z) * glm::highp_float(180.0/M_PI));

		// set computed values
		t.m_rotation = rotation;

		return t;
	}
}
