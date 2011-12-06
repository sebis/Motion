#include "Mesh.h"
#include "Shader.h"
#include "Trace.h"

#include <GL/glew.h>

namespace Common
{
	Mesh::Mesh()
	{
		// create single vertex array object and bind it
		glGenVertexArrays(1, &m_vaoID);
		glBindVertexArray(m_vaoID);

		// generate, bind and fill one interleaved vertex buffer object with all vertex attributes
		glGenBuffers(1, &m_vboID);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		//glBufferData(GL_ARRAY_BUFFER, vSize*sizeof(vertex), vData, GL_STATIC_DRAW);

		// create vertex attribute pointers into our buffer data with correct strides and offsets
		glVertexAttribPointer(Shader::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
		glEnableVertexAttribArray(Shader::POSITION);
		glVertexAttribPointer(Shader::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal)); 
		glEnableVertexAttribArray(Shader::NORMAL);
		glVertexAttribPointer(Shader::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, color));
		glEnableVertexAttribArray(Shader::COLOR);
		glVertexAttribPointer(Shader::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, texcoord));
		glEnableVertexAttribArray(Shader::TEXCOORD);

		// create and fill index buffer
		glGenBuffers(1, &m_eboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize*sizeof(glm::uint), iData, GL_STATIC_DRAW);

		// we're finished with the VAO so unbind it
		glBindVertexArray(0);
	}

	Mesh::~Mesh()
	{
		// make sure all buffers are unbound
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// delete buffers
		glDeleteBuffers(1, &m_eboID);
		glDeleteBuffers(1, &m_vboID);

		// same for VAO
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vaoID);
	}

	size_t Mesh::count() const
	{
		return m_indices.size();
	}

	void Mesh::draw()
	{
		// draw the mesh using the bound index buffer
		glBindVertexArray(m_vaoID);

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(vertex), &m_vertices[0], GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()*sizeof(unsigned int), &m_indices[0], GL_DYNAMIC_DRAW);

		glDrawElements(GL_TRIANGLES, count(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
