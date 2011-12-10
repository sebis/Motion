#ifndef COMMON_SOFTBODY
#define COMMON_SOFTBODY

#include "CollisionBody.h"
#include "MeshObject.h"
#include "Material.h"
#include "SoftBodyWorld.h"

#include "glm/glm.hpp"

#include <vector>
#include <set>

namespace Common
{
	class SoftBody : public CollisionBody
	{
	public:
		struct Node
		{
			Node(glm::vec3 & _position, float _mass) 
				: position(_position), 
				  mass(_mass), 
				  velocity(glm::vec3(0.0f)), 
				  force(glm::vec3(0.0f)),
				  oldPosition(_position),
				  constrained(false)
			{}

			float mass;
			glm::vec3 velocity;
			glm::vec3 force;
			glm::vec3 & position;
			glm::vec3 oldPosition;
			bool constrained;
		};

		struct Spring
		{
			Spring(Node * _n1, Node * _n2)
				: n1(_n1), n2(_n2)
			{
				restLength = glm::distance(n2->position, n1->position);
			}

			Node * n1;
			Node * n2;
			float restLength;
		};

		SoftBody(GameObject * gameObject);
		virtual ~SoftBody();

		void applyForces(float dt);
		void solveConstraints();
		void integrate(float dt);
		void resolveCollisions();

		void update(float dt);

		static MeshObject * createCloth(Material * material, SoftBodyWorld * world, SoftBody * body = 0);

	private:
		static const unsigned WIDTH;
		static const unsigned HEIGHT;

		Node * node(unsigned x, unsigned z);

		typedef std::vector<Node*> Nodes;
		typedef Nodes::iterator NodeIterator;
		Nodes m_nodes;

		typedef std::vector<Spring*> Springs;
		typedef Springs::iterator SpringIterator;
		Springs m_springs;

		GameObject * m_gameObject;
	};
}

#endif /* COMMON_SOFTBODY */