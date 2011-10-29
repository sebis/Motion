#ifndef INTERPOLATION_TRANSFORMRENDERER
#define INTERPOLATION_TRANSFORMRENDERER

#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "Transform.h"

namespace Interpolation
{
	class TransformRenderer : public Common::Renderer
	{
	public:
		TransformRenderer(Common::GameObject * gameObject, Shader * shader, const Common::Transform* transform);
		virtual ~TransformRenderer();

		virtual void draw();

	private:
		Shader * m_shader;

		const Common::Transform* m_transform;

		Mesh * m_yaw;
		Mesh * m_roll;
		Mesh * m_pitch;
	};
}

#endif /* INTERPOLATION_TRANSFORMRENDERER */
