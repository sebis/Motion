#ifndef COMMON_MESHOBJECT
#define COMMON_MESHOBJECT

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
	private:
		Mesh * m_mesh;
	};
};

#endif /* COMMON_MESHOBJECT */
