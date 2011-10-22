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
		MeshObject(Mesh * mesh, Material * material, Bone * bone = 0);
		virtual ~MeshObject();

		virtual glm::mat4 transformMatrix() const;

		inline void setBone(Bone * bone) { m_bone = bone; }

	private:
		Mesh * m_mesh;
		Bone * m_bone;
	};
};

#endif /* COMMON_MESHOBJECT */
