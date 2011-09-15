#ifndef COMMON_MESHRENDERER
#define COMMON_MESHRENDERER

#include "Mesh.h"
#include "Shader.h"
#include "Renderer.h"

namespace Common
{
	class MeshRenderer : public Renderer
	{
	public:
		MeshRenderer(GameObject * gameObject, Mesh * mesh, Shader * shader);
		virtual ~MeshRenderer() {}

		void draw();

	private:
		Mesh * m_mesh;
		Shader * m_shader;
	};
};

#endif /* COMMON_MESHRENDERER */