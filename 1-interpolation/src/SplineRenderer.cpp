#include "GameObject.h"
#include "SplineRenderer.h"
#include "Trace.h"

#include <vector>

namespace Interpolation
{
	SplineRenderer::SplineRenderer(Common::GameObject * gameObject, Interpolator<glm::vec3> * interpolator, Vertices vertices)
		: Renderer(gameObject),
		m_interpolator(interpolator),
		m_initialized(false),
		m_vertices(vertices),
		m_segments(20), // number of segments per second
		m_majorSize(10.0f),
		m_minorSize(5.0f),
		m_color(glm::vec4(1.0f))
	{
		m_lineShader = Shader::find("solid");
		m_pointShader = Shader::find("point");
	}

	void SplineRenderer::init()
	{
		if (m_initialized) {
			Trace::error("SplineRenderer already initialized!\n");
			return;
		}

		float t = 0.0f;
		float time = (m_vertices[m_vertices.count()-1].time - m_vertices[0].time);

		float dt = 1000.0f/(time*m_segments);

		for (int i = 0; i < m_vertices.count() - 1; i++) {
			vertex v(m_vertices[i].value, m_color, m_majorSize);
			vData.push_back(v);
			vPointData.push_back(v);

			for (int c = 0; t <= 1.0f; t += dt, c++) {
				int k = -1;
				float _t = m_vertices.get_k(t, k);
				if (i != k)
					break;

				vertex v(m_interpolator->interpolate(m_vertices, i, _t), m_color, m_minorSize);
				vData.push_back(v);
				// TODO: better control over dot placement
				if (c % m_segments == 0)
					vPointData.push_back(v);
			}
		}
		vertex v(m_vertices[m_vertices.count() - 1].value, m_color, m_majorSize);
		vData.push_back(v);
		vPointData.push_back(v);

		m_vertexCount = vData.size();
		m_pointCount = vPointData.size();

		// create single vertex array object and bind it
		glGenVertexArrays(2, m_vaoID);

		glBindVertexArray(m_vaoID[0]);

		// generate, bind and fill interleaved vertex buffer object with vertex attributes
		glGenBuffers(1, &m_vboID[0]);

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
		glBufferData(GL_ARRAY_BUFFER, m_vertexCount*sizeof(vertex), &vData[0], GL_DYNAMIC_DRAW);

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
		glBufferData(GL_ARRAY_BUFFER, m_pointCount*sizeof(vertex), &vPointData[0], GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, color));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, size));
		glEnableVertexAttribArray(3);

		// we're finished with the VAO so unbind it
		glBindVertexArray(0);

		m_initialized = true;
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
		if (!m_initialized) {
			Trace::error("SplineRenderer not initialized!\n");
			return;
		}

		// TODO: we don't need to reupload the data on each frame
		glBindVertexArray(m_vaoID[0]);

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
		glBufferData(GL_ARRAY_BUFFER, m_vertexCount*sizeof(vertex), &vData[0], GL_DYNAMIC_DRAW);

		m_lineShader->bind();
		m_lineShader->setUniform("world", glm::mat4(1.0f));
		m_lineShader->setUniform("view", m_gameObject->m_camera->view());
		m_lineShader->setUniform("projection", m_gameObject->m_camera->projection());
		glDrawArrays(GL_LINE_STRIP, 0, m_vertexCount);
		m_lineShader->unbind();

		glBindVertexArray(m_vaoID[1]);

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
		glBufferData(GL_ARRAY_BUFFER, m_pointCount*sizeof(vertex), &vPointData[0], GL_DYNAMIC_DRAW);

		m_pointShader->bind();
		m_pointShader->setUniform("world", glm::mat4(1.0f));
		m_pointShader->setUniform("view", m_gameObject->m_camera->view());
		m_pointShader->setUniform("projection", m_gameObject->m_camera->projection());
		glDrawArrays(GL_POINTS, 0, m_pointCount);
		m_pointShader->unbind();

		glBindVertexArray(0);
	}
}