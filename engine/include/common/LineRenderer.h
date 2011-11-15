#ifndef COMMON_LINERENDERER
#define COMMON_LINERENDERER

#include "Renderer.h"
#include "Shader.h"
#include "Utils.h"

#include "glm/glm.hpp"

#include <vector>

namespace Common
{
	class LineRenderer : public Renderer
	{
	public:
		struct vertex {
			glm::vec3 position;
			glm::vec4 color;
		};

		LineRenderer(GameObject * gameObject);

		virtual ~LineRenderer();

		inline void addLine(const vertex & a, const vertex & b)
		{
			m_vertices.push_back(a);
			m_vertices.push_back(b);

			Utils::print_vector("drawing", a.position);
		}

		inline void clear()
		{
			m_vertices.clear();
		}

		virtual void draw();

	private:
		std::vector<vertex> m_vertices;

		Shader * m_shader;

		glm::uint m_vaoID;
		glm::uint m_vboID;
	};
}

#endif /* COMMON_LINERENDERER */