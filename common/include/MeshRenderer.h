#ifndef COMMON_MESHRENDERER
#define COMMON_MESHRENDERER

#include "Material.h"
#include "Mesh.h"
#include "Renderer.h"

namespace Common
{
	class MeshRenderer : public Renderer
	{
	public:
		MeshRenderer(GameObject * gameObject, Mesh * mesh, Material * material);
		virtual ~MeshRenderer();

		void draw();

	private:
		Mesh * m_mesh;
		Material * m_material;
	};
};

#endif /* COMMON_MESHRENDERER */