#include "SplineRenderer.h"
#include "Trace.h"

#include <vector>

namespace Interpolation
{
	SplineRenderer::SplineRenderer(Common::GameObject * gameObject, Interpolator<glm::vec3> * interpolator, Vertices vertices)
		: Renderer(gameObject),
		m_interpolator(interpolator),
		m_vertices(vertices),
		m_majorSize(10.0f),
		m_minorSize(5.0f),
		m_color(glm::vec4(1.0f))
	{
		m_lineShader = Shader::find("solid");
		m_pointShader = Shader::find("point");

		const int segments = 10;
		float t = 1.0f/(segments+1);

		std::vector<SplineRenderer::vertex> vData;

		for (int i = 0; i < vertices.count() - 1; i++) {
			vData.push_back(vertex(vertices[i].value, m_color, m_majorSize));

			for (int j = 1; j <= segments; j++) {
				vData.push_back(vertex(m_interpolator->interpolate(vertices, i, t*j), m_color, m_minorSize));
			}
		}
		vData.push_back(vertex(vertices[vertices.count() - 1].value, m_color, m_majorSize));

		m_count = vData.size();

		// create single vertex array object and bind it
		glGenVertexArrays(1, &m_vaoID);
		glBindVertexArray(m_vaoID);

		// generate, bind and fill one interleaved vertex buffer object with all vertex attributes
		glGenBuffers(1, &m_vboID);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		glBufferData(GL_ARRAY_BUFFER, m_count*sizeof(vertex), &vData[0], GL_STATIC_DRAW);

		// create vertex attribute pointers into our buffer data with correct strides and offsets
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, color));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, size));
		glEnableVertexAttribArray(3);

		// we're finished with the VAO so unbind it
		glBindVertexArray(0);
	}

	SplineRenderer::~SplineRenderer()
	{
		// make sure all buffers are unbound
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// delete buffers
		glDeleteBuffers(1, &m_vboID);

		// same for VAO
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vaoID);
	}

	void SplineRenderer::draw()
	{
		glBindVertexArray(m_vaoID);

		m_lineShader->bind();
		m_lineShader->setUniform("world", glm::mat4(1.0f));
		glDrawArrays(GL_LINE_STRIP, 0, m_count);
		m_lineShader->unbind();

		m_pointShader->bind();
		m_pointShader->setUniform("world", glm::mat4(1.0f));
		glDrawArrays(GL_POINTS, 0, m_count);
		m_pointShader->unbind();

		glBindVertexArray(0);
	}
}