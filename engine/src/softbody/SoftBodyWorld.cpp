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
