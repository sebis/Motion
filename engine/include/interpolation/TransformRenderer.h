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
		TransformRenderer(Common::GameObject * gameObject, Common::Shader * shader, const Common::Transform* transform);
		virtual ~TransformRenderer();

		virtual void draw();

	private:
		Common::Shader * m_shader;

		const Common::Transform* m_transform;

		Common::Mesh * m_yaw;
		Common::Mesh * m_roll;
		Common::Mesh * m_pitch;
	};
}

#endif /* INTERPOLATION_TRANSFORMRENDERER */
