#ifndef COMMON_MESH
#define COMMON_MESH

#include "glm/glm.hpp"

#include <string>
#include <vector>

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

		typedef std::vector<Mesh::vertex> Vertices;
		typedef std::vector<unsigned int> Indices;

		Mesh();
		virtual ~Mesh();

		void draw();

		inline void setVertices(const Vertices & vertices) { m_vertices = vertices; }
		inline void setIndices(const Indices & indices) { m_indices = indices; }

		inline Vertices & vertices() { return m_vertices; }
		inline const Vertices & vertices() const { return m_vertices; }

	private:
		glm::uint m_vaoID;
		glm::uint m_vboID;
		glm::uint m_eboID;

		Vertices m_vertices;
		Indices m_indices;

		inline size_t count() const;
	};

	namespace MeshFactory
	{
		Mesh * Sphere(glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), int segments = 32);
		Mesh * Cube(bool override_color = true, glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		Mesh * Plane(glm::vec4 color = glm::vec4(1.0f), int gridSize = 1);
		Mesh * PlaneMesh(glm::vec4 color = glm::vec4(1.0f), int width = 10, int length = 10);
		void PlaneMesh(int width, int length, std::vector<Mesh::vertex> & vData, std::vector<glm::uint> & iData, glm::vec4 color = glm::vec4(1.0f));
		Mesh * FromFile(std::string str, glm::vec4 color = glm::vec4(1.0f));
	}
}

#endif /* COMMON_MESH */
