#ifndef INTERPOLATION_MESHOBJECT
#define INTERPOLATION_MESHOBJECT

#include "GameObject.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

namespace Interpolation
{
	class MeshObject : public Common::GameObject
	{
	public:
		MeshObject(Shader * shader, Mesh * mesh, Common::Texture * texture = 0);
		MeshObject(Shader * shader, std::string file, std::string texture = "");
		virtual ~MeshObject();
	private:
		Mesh * m_mesh;
	};
};

#endif /* INTERPOLATION_MESHOBJECT */