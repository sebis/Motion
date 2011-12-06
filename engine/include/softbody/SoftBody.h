#ifndef COMMON_SOFTBODY
#define COMMON_SOFTBODY

#include "MeshObject.h"
#include "Material.h"
#include "SoftBodyWorld.h"

#include "glm/glm.hpp"

#include <vector>
#include <set>

namespace Common
{
	class SoftBody
	{
	public:
		struct Node
		{
			Node(glm::vec3 & _position, float _mass) 
				: position(_position), 
				  mass(_mass), 
				  velocity(glm::vec3(0.0f)), 
				  force(glm::vec3(0.0f)),
				  dv0(glm::vec3(0.0f)),
				  dv1(glm::vec3(0.0f)),
				  constrained(false)
			{}

			float mass;
			glm::vec3 velocity;
			glm::vec3 force;
			glm::vec3 & position;
			glm::vec3 dv0;
			glm::vec3 dv1;
			bool constrained;

			typedef std::set<Node*> Links;
			typedef Links::iterator LinkIterator;
			Links links;
		};

		struct Spring
		{
			Spring(Node * _n1, Node * _n2, float _restLength)
				: n1(_n1),
				  n2(_n2),
				  restLength(_restLength)
			{}

			Node * n1;
			Node * n2;
			float restLength;
		};

		SoftBody(GameObject * gameObject);
		virtual ~SoftBody();

		void update(float dt);

		static MeshObject * createCloth(Material * material, SoftBodyWorld * world);

	private:
		typedef std::vector<Node*> Nodes;
		typedef Nodes::iterator NodeIterator;
		Nodes m_nodes;

		typedef std::vector<Spring*> Springs;
		typedef Springs::iterator SpringIterator;
		Springs m_springs;
	};
}

#endif /* COMMON_SOFTBODY */