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

		Interpolator<glm::vec3> * interpolator = new KochanekBartelsInterpolator<glm::vec3>;
		KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(this, interpolator, m_transform.position(), true);

		animator->addKeyframe(0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 0, -1, 0);
		animator->addKeyframe(2000.0f, glm::vec3(-2.5f, 0.0f, 2.0f), 0, -1, 0);
		animator->addKeyframe(6000.0f, glm::vec3(5.0f, 0.0f, 5.0f), 0, -1, 0);
		animator->addKeyframe(7000.0f, glm::vec3(3.0f, 0.0f, 3.0f), 0, -1, 0);
		animator->addKeyframe(13000.0f, glm::vec3(-1.5f, 0.0f, 8.0f), 0, -1, 0);
		animator->addKeyframe(15000.0f, glm::vec3(-4.0f, 0.0f, 7.0f), 0, -1, 0);
		animator->addKeyframe(16000.0f, glm::vec3(-5.0f, 0.0f, -7.0f), 0, -1, 0);
		animator->addKeyframe(20000.0f, glm::vec3(5.0f, 0.0f, -7.0f), 0, -1, 0);

		// TODO: should not need to set spline renderer AFTER keyframes
		animator->setRenderer(new SplineRenderer(this, interpolator, *animator));

		m_animator = animator;
	}

	CubeObject::~CubeObject()
	{
		delete m_mesh;
	}
};