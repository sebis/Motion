#ifndef COMMON_MESHRENDERER
#define COMMON_MESHRENDERER

#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

namespace Common
{
	class MeshRenderer : public Renderer
	{
	public:
		MeshRenderer(GameObject * gameObject, Mesh * mesh, Shader * shader, Texture * texture = 0);
		virtual ~MeshRenderer();

		void draw();

	private:
		Mesh * m_mesh;
		// TODO: create a material class
		Shader * m_shader;
		Texture * m_texture;
	};
};

#endif /* COMMON_MESHRENDERER */