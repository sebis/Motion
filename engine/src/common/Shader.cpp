#include "Shader.h"
#include "Trace.h"
#include "Utils.h"

#include "glm/gtc/type_ptr.hpp"

namespace Common
{
	Shader::Shader()
	: num_ids(0)
	{
		// create shader program
		p_id = glCreateProgram();
	}

	Shader::~Shader()
	{
		for (unsigned i = 0; i < num_ids; i++)
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

		// query information about the linked program
		/*int num_active;
		int max_length;

		glGetProgramiv(p_id, GL_ACTIVE_ATTRIBUTES, &num_active);
		glGetProgramiv(p_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);

		char * attr_name = new char[max_length];

		// loop through all active attributes
		for (int i = 0; i < num_active; i++)
		{
			int attrib_size;
			GLenum attrib_type;

			glGetActiveAttrib(p_id, i, max_length, 0, &attrib_size, &attrib_type, attr_name);
			Trace::info("Found GLSL attibute %s\n", attr_name);
		}

		delete [] attr_name;*/
	}

	void Shader::bindAttribute(GLuint index, const GLchar* name)
	{
		glBindAttribLocation(p_id, index, name);
	}

	bool Shader::load(std::string filename, GLenum shaderType)
	{
		id_table[num_ids] = glCreateShader(shaderType);
		int id = id_table[num_ids++];

		std::string source = Utils::readfile(filename);

		const GLchar* sstring = source.c_str();
		const GLint slength = source.length();
		glShaderSource(id, 1, &sstring, &slength);

		if (!compile(id))
			return false;

		glAttachShader(p_id, id);
		return true;
	}

	bool Shader::compile(unsigned int id)
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

			return false;
		}
	
		Trace::info("Shader compile succeeded!\n");
		return true;
	}

	void Shader::setUniform(const GLchar *name, const glm::vec3& v)
	{
		glUniform3fv(glGetUniformLocation(p_id, name), 1, glm::value_ptr(v));
	}

	void Shader::setUniform(const GLchar *name, const glm::vec4& v)
	{
		glUniform4fv(glGetUniformLocation(p_id, name), 1, glm::value_ptr(v));
	}

	void Shader::setUniform(const GLchar *name, const glm::mat4& m)
	{
		glUniformMatrix4fv(glGetUniformLocation(p_id, name), 1, GL_FALSE, glm::value_ptr(m));
	}

	void Shader::setUniform(const GLchar *name, const glm::float_t& f)
	{
		glUniform1f(glGetUniformLocation(p_id, name), f);
	}

	void Shader::setUniform(const GLchar *name, bool b)
	{
		glUniform1i(glGetUniformLocation(p_id, name), b);
	}

	void Shader::bind() const
	{
		glUseProgram(p_id);
	}

	void Shader::unbind() const
	{
		glUseProgram(0);
	}
}