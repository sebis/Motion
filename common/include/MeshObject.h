#ifndef COMMON_MESHOBJECT
#define COMMON_MESHOBJECT

#include "GameObject.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

namespace Common
{
	class MeshObject : public GameObject
	{
	public:
		MeshObject(Shader * shader, Mesh * mesh, Texture * texture = 0);
		MeshObject(Shader * shader, std::string file, std::string texture = "");
		virtual ~MeshObject();
	private:
		Mesh * m_mesh;
	};
};

#endif /* COMMON_MESHOBJECT */
