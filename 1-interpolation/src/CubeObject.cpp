#include "CubeObject.h"
#include "Interpolator.h"
#include "KeyframeAnimator.h"
#include "MeshRenderer.h"
#include "Transform.h"

using namespace Common;

namespace Interpolation
{
	CubeObject::CubeObject(Shader * shader)
	{
		m_mesh = MeshFactory::Cube(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		m_renderer = new MeshRenderer(this, m_mesh, shader);
	}

	CubeObject::~CubeObject()
	{
		delete m_mesh;
	}
};