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

#ifndef COMMON_COLLISIONBODY
#define COMMON_COLLISIONBODY

namespace Common
{
	class CollisionBody
	{
	public:
		enum Type {
			RIGID,
			SOFT,
		};

		CollisionBody(Type type = RIGID) : m_type(type) {}
		virtual ~CollisionBody() {}
	
		inline Type type() const { return m_type; }

	private:
		Type m_type;
	};
}

#endif /* COMMON_COLLISIONBODY */