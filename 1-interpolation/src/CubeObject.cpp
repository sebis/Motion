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

		LinearInterpolator<glm::vec3> * interpolator = new LinearInterpolator<glm::vec3>;

		LocationKeyframeAnimator * animator = new LocationKeyframeAnimator(this, interpolator, m_transform.m_position);

		/*animator->addKeyframe(0.0f, Transform::Translation(0.0f, 0.0f, 0.0f));
		animator->addKeyframe(4000.0f, Transform::Translation(0.0f, 0.0f, 5.0f));
		animator->addKeyframe(8000.0f, Transform::Translation(5.0f, 0.0f, 5.0f));*/
		animator->addKeyframe(0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
		animator->addKeyframe(4000.0f, glm::vec3(0.0f, 0.0f, 5.0f));
		animator->addKeyframe(8000.0f, glm::vec3(5.0f, 0.0f, 5.0f));

		m_animator = animator;
	}

	CubeObject::~CubeObject()
	{
		delete m_mesh;
	}
};