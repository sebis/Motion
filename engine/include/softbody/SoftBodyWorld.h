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
		typedef std::vector<SoftBody *> Container;
		typedef Container::iterator Iterator;
		Container m_bodies;
	};
}

#endif /* COMMON_SOFTBODYWORLD */
