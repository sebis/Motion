#include "Mesh.h"
#include "Trace.h"

#include <gl/glew.h>

Mesh::Mesh(Mesh::vertex vData[], size_t vSize, glm::uint iData[], size_t iSize)
{
	// store number of drawable elements
	m_count = iSize;

	// create single vertex array object and bind it
	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);

	// generate, bind and fill one interleaved vertex buffer object with all vertex attributes
	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, vSize*sizeof(vertex), vData, GL_STATIC_DRAW);

	// create vertex attribute pointers into our buffer data with correct strides and offsets
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, color));
	glEnableVertexAttribArray(2);

	// create and fill index buffer
	glGenBuffers(1, &m_eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize*sizeof(glm::uint), iData, GL_STATIC_DRAW);

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

void Mesh::draw()
{
	// draw the mesh using the bound index buffer
	glBindVertexArray(m_vaoID);
	glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}