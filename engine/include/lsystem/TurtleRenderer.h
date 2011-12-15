#ifndef COMMON_TURTLERENDERER
#define COMMON_TURTLERENDERER

#include "MeshRenderer.h"
#include "LSystem.h"

#include <stack>
#include <vector>
#include <queue>
#include <unordered_map>

namespace Common
{
	class TurtleRenderer : public Renderer
	{
	public:
		TurtleRenderer(LSystem * system);
		virtual ~TurtleRenderer();

		virtual void draw();

	private:
		void drawLeaf(int level, int branch);
		void drawSegment(int level, int branch);
		void parseSystem();

		LSystem * m_system;
		Material * m_material;
		Material * m_leafMaterial;
		Mesh * m_mesh;
		Mesh * m_leafMesh;

		std::stack<glm::mat4> m_stack;

		typedef std::vector<glm::mat4> Branch;
		typedef std::unordered_map<unsigned, Branch> Branches;
		typedef std::map<unsigned, Branches> Container;

		Container m_leaves;
		Container m_branches;
	};
}

#endif /* COMMON_TURTLERENDERER */