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
		TurtleRenderer(LSystem * system);
		virtual ~TurtleRenderer();

		virtual void draw();

	private:
		void drawLeaf();
		void drawSegment(int level);
		void parseSystem();

		LSystem * m_system;
		Material * m_material;
		Material * m_leafMaterial;
		Mesh * m_mesh;
		Mesh * m_leafMesh;

		std::stack<glm::mat4> m_stack;

		std::vector<glm::mat4> m_leaves;
		std::vector<glm::mat4> m_branches;
	};
}

#endif /* COMMON_TURTLERENDERER */