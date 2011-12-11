#ifndef COMMON_TURTLERENDERER
#define COMMON_TURTLERENDERER

#include "MeshRenderer.h"
#include "LSystem.h"

#include <stack>

namespace Common
{
	class TurtleRenderer : public Renderer
	{
	public:
		TurtleRenderer(GameObject * gameObject, LSystem * system);
		virtual ~TurtleRenderer();

		virtual void draw();

	private:
		void drawSegment();
		void parseSystem();

		LSystem * m_system;
		Material * m_material;
		Mesh * m_mesh;

		std::stack<glm::mat4> m_stack;
	};
}

#endif /* COMMON_TURTLERENDERER */