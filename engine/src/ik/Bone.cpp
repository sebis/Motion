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

#include "Bone.h"

namespace Common
{
	Bone::Bone(const std::string& name, Transform& transform, Bone * parent)
		: m_name(name), m_transform(transform), m_parent(parent)
	{
		// Initialize values
		m_axes[0].setAxis(glm::vec3(1.0f, 0.0f, 0.0f));
		m_axes[0].setConstraints(0.0f, 0.0f);
		m_axes[1].setAxis(glm::vec3(0.0f, 1.0f, 0.0f));
		m_axes[1].setConstraints(0.0f, 0.0f);
		m_axes[2].setAxis(glm::vec3(0.0f, 0.0f, 1.0f));
		m_axes[2].setConstraints(0.0f, 0.0f);
	}
}
