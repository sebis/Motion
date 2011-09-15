#include "Shader.h"
#include "Trace.h"
#include "Utils.h"

#include "glm/gtc/type_ptr.hpp"

Shader::Shader(const std::string vs_filename, const std::string fs_filename)
: num_ids(0)
{
	// create shader program
	p_id = glCreateProgram();

	// load appropriate sources
	load(vs_filename, GL_VERTEX_SHADER);
	load(fs_filename, GL_FRAGMENT_SHADER);
}

Shader::~Shader()
{
	for (int i = 0; i < num_ids; i++)
	{
		glDetachShader(p_id, id_table[i]);
		glDeleteShader(id_table[i]);
	}

	glDeleteProgram(p_id);
}

void Shader::link()
{
	// link the program
	glLinkProgram(p_id);
}

void Shader::bindAttribLocation(GLuint index, const GLchar* name)
{
	glBindAttribLocation(p_id, index, name);
}

void Shader::load(std::string filename, GLenum shaderType)
{
	id_table[num_ids] = glCreateShader(shaderType);
	int id = id_table[num_ids++];

	std::string source = Utils::readfile(filename);

	const GLchar* sstring = source.c_str();
	const GLint slength = source.length();
	glShaderSource(id, 1, &sstring, &slength);

	compile(id);

	glAttachShader(p_id, id);
}

void Shader::compile(unsigned int id)
{
	GLint compiled;

	// compile shader
	glCompileShader(id);

	// check for compile status
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint length;
		char* log;

		// retreive log length
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		
		if (length > 0)
		{
			// retreive log contents and output it to cout
			log = new char[length];
			glGetShaderInfoLog(id, length, 0, log);
			Trace::error("Shader compile failed: %s\n", log);
			delete[] log;
		}
	}
	else
	{
		Trace::info("Shader compile succeeded!\n");
	}
}

void Shader::setUniform(const GLchar *name, const glm::vec3& v)
{
	glUniform3fv(glGetUniformLocation(p_id, name), 1, glm::value_ptr(v));
}

void Shader::setUniform(const GLchar *name, const glm::mat4& m)
{
	glUniformMatrix4fv(glGetUniformLocation(p_id, name), 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::bind() const
{
	glUseProgram(p_id);
}

void Shader::unbind() const
{
	glUseProgram(0);
}