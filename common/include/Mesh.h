#ifndef COMMON_MESH
#define COMMON_MESH

#include "glm/glm.hpp"

#include <string>

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

namespace Common
{
	namespace MeshFactory
	{
		Mesh * Cube(glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		Mesh * FromFile(std::string str);
	}
}

#endif /* COMMON_MESH */