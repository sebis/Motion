#include "MeshObject.h"
#include "MeshRenderer.h"

namespace Common
{
	MeshObject::MeshObject(Mesh * mesh, Material * material)
	{
		m_mesh = mesh;
		m_renderer = new MeshRenderer(this, m_mesh, material);
	}

	MeshObject::~MeshObject()
	{
		delete m_mesh;
	}
}
