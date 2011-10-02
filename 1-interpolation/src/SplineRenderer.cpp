#include "SplineRenderer.h"
#include "Trace.h"

#include <vector>

namespace Interpolation
{
	SplineRenderer::SplineRenderer(Common::GameObject * gameObject, Interpolator<glm::vec3> * interpolator, Vertices vertices)
		: Renderer(gameObject),
		m_interpolator(interpolator),
		m_segments(5),
		m_majorSize(10.0f),
		m_minorSize(5.0f),
		m_color(glm::vec4(1.0f))
	{
		m_lineShader = Shader::find("solid");
		m_pointShader = Shader::find("point");

		std::vector<SplineRenderer::vertex> vData;
		std::vector<SplineRenderer::vertex> vPointData;

		float t = 0.0f;
		float time = (vertices[vertices.count()-1].time - vertices[0].time);
		float dt = (1000.0f/m_segments)/time;

		for (int i = 0; i < vertices.count() - 1; i++) {
			vData.push_back(vertex(vertices[i].value, m_color, m_majorSize));
			//t += dt;
			vPointData.push_back(vData.back());

			int k = -1;

			for (; t <= 1.0f; t += dt) {
				float _t = vertices.get_k(t, k);
				if (i != k)
					break;
				vData.push_back(vertex(m_interpolator->interpolate(vertices, i, _t), m_color, m_minorSize));
				vPointData.push_back(vertex(m_interpolator->interpolate(vertices, i, _t), m_color, m_minorSize));
				//t += dt;
			}

			/*for (int j = 0; j < m_segments; j++) {
				float _t = j/(m_segments-1);
				vData.push_back(vertex(m_interpolator->interpolate(vertices, i, _t), m_color, m_minorSize));
			}*/


			//float dt = (vertices[vertices.count() - 1].time - vertices[0].time) / 500.0f;
			/*float dt2 = (vertices[i+1].time - vertices[i].time) / 500.0f;
			float t2 = 1.0f/dt2;

			for (int j = 0; j <= dt; j++) {
				vPointData.push_back(vertex(m_interpolator->interpolate(vertices, i, t2*j), m_color, m_minorSize));
			}*/
		}
		vData.push_back(vertex(vertices[vertices.count() - 1].value, m_color, m_majorSize));
		vPointData.push_back(vData.back());

		m_vertexCount = vData.size();
		m_pointCount = vPointData.size();

		// create single vertex array object and bind it
		glGenVertexArrays(2, m_vaoID);

		glBindVertexArray(m_vaoID[0]);

		// generate, bind and fill interleaved vertex buffer object with vertex attributes
		glGenBuffers(1, &m_vboID[0]);

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
		glBufferData(GL_ARRAY_BUFFER, m_vertexCount*sizeof(vertex), &vData[0], GL_STATIC_DRAW);

		// create vertex attribute pointers into our buffer data with correct strides and offsets
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, color));
		glEnableVertexAttribArray(2);

		// bind the next VAO
		glBindVertexArray(m_vaoID[1]);

		// generate a buffer for vertex data
		glGenBuffers(1, &m_vboID[1]);

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
		glBufferData(GL_ARRAY_BUFFER, m_pointCount*sizeof(vertex), &vPointData[0], GL_STATIC_DRAW);

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
		glDeleteBuffers(2, m_vboID);

		// same for VAO
		glBindVertexArray(0);
		glDeleteVertexArrays(2, m_vaoID);
	}

	void SplineRenderer::draw()
	{
		glBindVertexArray(m_vaoID[0]);

		m_lineShader->bind();
		m_lineShader->setUniform("world", glm::mat4(1.0f));
		glDrawArrays(GL_LINE_STRIP, 0, m_vertexCount);
		m_lineShader->unbind();

		glBindVertexArray(m_vaoID[1]);

		m_pointShader->bind();
		m_pointShader->setUniform("world", glm::mat4(1.0f));
		glDrawArrays(GL_POINTS, 0, m_pointCount);
		m_pointShader->unbind();

		glBindVertexArray(0);
	}
}