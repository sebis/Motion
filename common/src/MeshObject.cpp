#include "MeshObject.h"
#include "MeshRenderer.h"

namespace Common
{
	MeshObject::MeshObject(Mesh * mesh, Material * material, Bone * bone)
		: m_mesh(mesh), m_bone(bone)
	{
		m_renderer = new MeshRenderer(this, mesh, material);
	}

	glm::mat4 MeshObject::transformMatrix() const
	{
		if (m_bone)
			return m_bone->absoluteTransform();

		return GameObject::transformMatrix();
	}

	MeshObject::~MeshObject()
	{
		delete m_mesh;
	}
}
