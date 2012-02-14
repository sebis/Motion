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

#ifndef COMMON_MESHOBJECT
#define COMMON_MESHOBJECT

#include "Bone.h"
#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"

namespace Common
{
	class MeshObject : public GameObject
	{
	public:
		MeshObject(Mesh * mesh, Material * material);
		virtual ~MeshObject();

		inline Mesh * mesh() const { return m_mesh; }

	private:
		Mesh * m_mesh;
	};
};

#endif /* COMMON_MESHOBJECT */
