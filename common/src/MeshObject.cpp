#include "MeshObject.h"
#include "MeshRenderer.h"

namespace Common
{
	MeshObject::MeshObject(Shader * shader, Mesh * mesh, Texture * texture)
	{
		m_mesh = mesh;
		m_renderer = new MeshRenderer(this, m_mesh, shader, texture);
	}

	MeshObject::MeshObject(Shader * shader, std::string file, std::string texture)
	{
		m_mesh = MeshFactory::FromFile(file);
		m_renderer = new MeshRenderer(this, m_mesh, shader, texture != "" ? new Texture(texture.c_str()) : 0);
	}

	MeshObject::~MeshObject()
	{
		delete m_mesh;
	}
}
