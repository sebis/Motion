#ifndef INTERPOLATION_CUBEOBJECT
#define INTERPOLATION_CUBEOBJECT

#include "GameObject.h"
#include "Mesh.h"
#include "Shader.h"

namespace Interpolation
{
	class CubeObject : public Common::GameObject
	{
	public:
		CubeObject(Shader * shader);
		virtual ~CubeObject();
	private:
		Mesh * m_mesh;
	};
};

#endif /* INTERPOLATION_CUBEOBJECT */