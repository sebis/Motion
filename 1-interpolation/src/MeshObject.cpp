#include "MeshObject.h"
#include "Interpolator.h"
#include "KeyframeAnimator.h"
#include "MeshRenderer.h"
#include "Transform.h"

using namespace Common;

namespace Interpolation
{
	MeshObject::MeshObject(Shader * shader, Mesh * mesh)
	{
		m_mesh = mesh;
		m_renderer = new MeshRenderer(this, m_mesh, shader);
	}

	MeshObject::MeshObject(Shader * shader, std::string file)
	{
		m_mesh = MeshFactory::FromFile(file);
		m_renderer = new MeshRenderer(this, m_mesh, shader);
	}

	MeshObject::~MeshObject()
	{
		delete m_mesh;
	}
};