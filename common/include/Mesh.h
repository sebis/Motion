#ifndef COMMON_MESH
#define COMMON_MESH

#include "glm/glm.hpp"

class Mesh
{
public:
	struct vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
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
	static Mesh * Cube(glm::vec4 color)
	{
		Mesh::vertex vData[] =
		{
			// front face 0,1,2,3
			{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },

			// right face 4,5,6,7
			{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), color },
			{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), color },
			{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), color },
			{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), color },

			// back face 8, 9, 10, 11
			{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },
			{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },
			{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },
			{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },

			// left face 12, 13, 14, 15
			{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), color },
			{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), color },
			{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), color },
			{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), color },

			// top face 16, 17, 18, 19
			{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), color },
			{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), color },
			{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), color },
			{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), color },

			// bottom face 20, 21, 22, 23
			{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), color },
			{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), color },
			{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), color },
			{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), color },
		};

		glm::uint iData[] =
		{
			0, 1, 2, 2, 1, 3,
			4, 5, 6, 6, 5, 7,
			8, 9, 10, 10, 9, 11,
			12, 13, 14, 14, 13, 15,
			16, 17, 18, 18, 17, 19,
			20, 21, 22, 22, 21, 23,
		};

		return new Mesh(vData, 24, iData, 36);
	}
};

#endif /* COMMON_MESH */