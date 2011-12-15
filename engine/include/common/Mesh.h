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

		Mesh(bool staticDraw = false);
		virtual ~Mesh();

		void draw();

		void setVertices(const Vertices & vertices);
		void setIndices(const Indices & indices);

		inline Vertices & vertices() { return m_vertices; }
		inline const Vertices & vertices() const { return m_vertices; }

		inline Mesh::vertex & vertexAt(unsigned int index) { return m_vertices[index]; }
		inline const Mesh::vertex & vertexAt(unsigned int index) const { return m_vertices[index]; }

		inline const Indices & indices() const { return m_indices; }

	private:
		void fillBuffer();

		glm::uint m_vaoID;
		glm::uint m_vboID;
		glm::uint m_eboID;

		Vertices m_vertices;
		Indices m_indices;

		bool m_staticDraw;

		inline size_t count() const;
	};

	namespace MeshFactory
	{
		void setStaticDraw(bool useStaticDraw);

		Mesh * Sphere(glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), int segments = 32);
		Mesh * Cube(bool override_color = true, glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		Mesh * Cylinder(int segments = 12, float radius = 1.0f, const glm::vec4 & color = glm::vec4(1.0f));
		Mesh * Plane(glm::vec4 color = glm::vec4(1.0f), int gridSize = 1);
		Mesh * PlaneMesh(glm::vec4 color = glm::vec4(1.0f), int width = 10, int length = 10);
		void PlaneMesh(int width, int length, std::vector<Mesh::vertex> & vData, std::vector<glm::uint> & iData, glm::vec4 color = glm::vec4(1.0f));
		Mesh * FromFile(std::string str, glm::vec4 color = glm::vec4(1.0f));
	}
}

#endif /* COMMON_MESH */
