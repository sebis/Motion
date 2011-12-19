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
			Node(glm::mat4 _m = glm::mat4(1.0f)) : m(_m), state(INIT), birthTime(0.0f), scale(1.0f), parent(0), isLeaf(false) {}

			glm::mat4 m;

			State state;
			float birthTime;

			float scale;

			Node * parent;
			std::vector<Node*> children;

			bool isLeaf;
		};

		Node * m_root;

		Node * drawLeaf(Node * parent);
		Node * drawSegment(Node * parent);
		void parseSystem();

		LSystem * m_system;
		Material * m_material;
		Material * m_leafMaterial;
		Mesh * m_mesh;
		Mesh * m_leafMesh;

		std::stack<glm::mat4> m_stack;
	};
}

#endif /* COMMON_TURTLERENDERER */