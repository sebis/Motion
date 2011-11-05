#ifndef COMMON_MESH
#define COMMON_MESH

#include "glm/glm.hpp"

#include <string>

namespace Common
{
	class Mesh
	{
	public:
		struct vertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texcoord;
			glm::vec4 color;
		};

		Mesh(vertex vData[], size_t vSize, glm::uint iData[], size_t iSize);
		virtual ~Mesh();

		void draw();

	private:
		glm::uint m_vaoID;
		glm::uint m_vboID;
		glm::uint m_eboID;
		size_t m_count;
	};

	namespace MeshFactory
	{
		Mesh * Sphere(glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), int segments = 32);
		Mesh * Cube(bool override_color = true, glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		Mesh * Plane(glm::vec4 color = glm::vec4(1.0f), int gridSize = 1);
		Mesh * FromFile(std::string str, glm::vec4 color = glm::vec4(1.0f));
	}
}

#endif /* COMMON_MESH */
