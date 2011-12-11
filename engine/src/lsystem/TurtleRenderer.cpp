#include "GameObject.h"
#include "TurtleRenderer.h"

namespace Common
{
	TurtleRenderer::TurtleRenderer(LSystem * system)
		: Renderer(0), m_system(system)
	{
		m_material = new Material(Shader::find("shader"));
		m_material->setDiffuseColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_mesh = MeshFactory::Cube(false, glm::vec4(0.8f, 0.5f, 0.25f, 1.0f));
	}

	TurtleRenderer::~TurtleRenderer()
	{
	}

	void TurtleRenderer::drawSegment(int level)
	{
		const glm::mat4 & m = m_stack.top();
		
		glm::mat4 temp = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.25f*0.5f, 0.01f)), glm::vec3(0.0f, 1.0f, 0.0f));

		m_material->shader()->setUniform("world", m * temp);

		m_mesh->draw();
	}

	void TurtleRenderer::parseSystem()
	{
		m_stack = std::stack<glm::mat4>();
		m_stack.push(glm::mat4(1.0f));
		int level = 0;

		std::string path = m_system->get();

		float angle = 25.0f;
		float offset = 0.25f;

		glm::mat4 m, top;

		for (int i = 0; i < path.length(); i++)
		{
			char chr = path[i];
			switch (chr)
			{
			case 'X':
				// ignored symbol
				break;
			case 'F':
				drawSegment(level);
				m = glm::translate(m_stack.top(), glm::vec3(0.0f, offset, 0.0f));
				m_stack.pop();
				m_stack.push(m);
				break;
			case '+':
				m = glm::rotate(m_stack.top(), angle, glm::vec3(0.0f, 0.0f, 1.0f));
				m_stack.pop();
				m_stack.push(m);
				break;
			case '-':
				m = glm::rotate(m_stack.top(), -angle, glm::vec3(0.0f, 0.0f, 1.0f));
				m_stack.pop();
				m_stack.push(m);
				break;
			case '!':
				m = glm::rotate(m_stack.top(), angle, glm::vec3(0.0f, 1.0f, 0.0f));
				m_stack.pop();
				m_stack.push(m);
			case '?':
				m = glm::rotate(m_stack.top(), -angle, glm::vec3(0.0f, 1.0f, 0.0f));
				m_stack.pop();
				m_stack.push(m);
			case '&':
				m = glm::rotate(m_stack.top(), angle, glm::vec3(1.0f, 0.0f, 0.0f));
				m_stack.pop();
				m_stack.push(m);
				break;
			case '/':
				m = glm::rotate(m_stack.top(), -angle, glm::vec3(1.0f, 0.0f, 0.0f));
				m_stack.pop();
				m_stack.push(m);
				break;
			case '[':
				m_stack.push(m_stack.top());
				level++;
				break;
			case ']':
				m_stack.pop();
				level--;
				break;
			default:
				Trace::info("Encountered unkown turtle command: %c\n", chr);
			}
		}
	}

	void TurtleRenderer::draw()
	{
		glDisable(GL_BLEND);
		m_material->begin();

		m_material->shader()->setUniform("view", GameObject::s_camera->view());
		m_material->shader()->setUniform("projection", GameObject::s_camera->projection());

		parseSystem();

		m_material->end();
		glEnable(GL_BLEND);
	}
}
