#ifndef INTERPOLATION_SPLINERENDERER
#define INTERPOLATION_SPLINERENDERER

#include "ControlPoints.h"
#include "Interpolator.h"
#include "Renderer.h"
#include "Shader.h"

#include "glm/glm.hpp"

namespace Interpolation
{
	class SplineRenderer : public Common::Renderer
	{
	private:
		struct vertex {
			glm::vec3 position;
			glm::vec4 color;
			glm::float_t size;

			vertex(glm::vec3 _position, glm::vec4 _color, glm::float_t _size)
				: position(_position), color(_color), size(_size) {}
		};

		typedef const ControlPoints<Keyframe<glm::vec3> >& Vertices;

	public:
		SplineRenderer(Common::GameObject * gameObject, Interpolator<glm::vec3> * interpolator, Vertices vertices);
		virtual ~SplineRenderer();

		void processMessage(std::string, void*);
		void draw();
		
	private:
		Shader * m_lineShader;
		Shader * m_pointShader;

		Interpolator<glm::vec3> * m_interpolator;

		Vertices m_vertices;
		size_t m_vertexCount;
		size_t m_pointCount;

		glm::uint m_vaoID[2];
		glm::uint m_vboID[2];

		const unsigned m_segments;
		float m_majorSize;
		float m_minorSize;
		glm::vec4 m_color;

	};
};

#endif /* INTERPOLATION_SPLINERENDERER */