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

		enum State
		{
			INIT,
			GROWING,
			FINISHED
		};

		struct Node
		{
			Node(glm::mat4 _m = glm::mat4(1.0f)) : m(_m), state(INIT), parent(0) {}
			glm::mat4 m;
			State state;
			Node * parent;
			std::vector<Node*> children;
		};

		Node * m_root;

		void drawLeaf(int level, int branch);
		Node * drawSegment(Node * parent);
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