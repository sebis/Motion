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
