#ifndef COMMON_SHADER
#define COMMON_SHADER

#include "Trace.h"

#include <string>
#include <map>
#include <GL/glew.h>

#include "glm/glm.hpp"

class Shader
{
private:
	Shader();

	unsigned id_table[10];
	unsigned num_ids;

	unsigned vs_id;
	unsigned fs_id;
	unsigned p_id;

	bool load(std::string filename, GLenum shaderType);
	bool compile(unsigned id);
	void link();

public:
	virtual ~Shader();

	void bind() const;
	void unbind() const;

	void bindAttribLocation(GLuint index, const GLchar* name);

	void setUniform(const GLchar *name, const glm::vec3& v);
	void setUniform(const GLchar *name, const glm::vec4& v);
	void setUniform(const GLchar *name, const glm::mat4& m);

	static Shader* find(std::string technique)
	{
		static std::map<std::string, Shader*> shaders;

		std::map<std::string, Shader*>::iterator it = shaders.find(technique);
		if (it != shaders.end())
			return it->second;

		Shader* shader = new Shader;

		static const std::string path("resources/");

		if (!shader->load(path + technique + ".vert", GL_VERTEX_SHADER)) {
			Trace::error("Could not find %s.vert\n", path + technique);
			return 0;
		}
		if (!shader->load(path + technique + ".frag", GL_FRAGMENT_SHADER)) {
			Trace::error("Could not find %s.frag\n", path + technique);
			return 0;
		}

		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			Trace::error("OpenGL error: %d\n", err);

		shader->bindAttribLocation(0, "in_Position");
		shader->bindAttribLocation(1, "in_Normal");
		shader->bindAttribLocation(2, "in_Color");
		shader->bindAttribLocation(3, "in_Size");

		err = glGetError();
		if (err != GL_NO_ERROR)
			Trace::error("OpenGL error: %d\n", err);

		shader->link();

		shaders[technique] = shader;
		return shader;
	}
};

#endif /* COMMON_SHADER */
