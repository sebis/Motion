#ifndef INTERPOLATION_MESHOBJECT
#define INTERPOLATION_MESHOBJECT

#include "GameObject.h"
#include "Mesh.h"
#include "Shader.h"

namespace Interpolation
{
	class MeshObject : public Common::GameObject
	{
	public:
		MeshObject(Shader * shader, Mesh * mesh);
		MeshObject(Shader * shader, std::string file);
		virtual ~MeshObject();
	private:
		Mesh * m_mesh;
	};
};

#endif /* INTERPOLATION_MESHOBJECT */