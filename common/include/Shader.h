#ifndef COMMON_SHADER
#define COMMON_SHADER

#include <string>
#include <GL/glew.h>

#include "glm/glm.hpp"

class Shader
{
private:
	unsigned id_table[10];
	unsigned num_ids;

	unsigned vs_id;
	unsigned fs_id;
	unsigned p_id;

	void compile(unsigned id);
public:
	Shader(std::string, std::string);
	virtual ~Shader();

	void bind() const;
	void unbind() const;

	void load(std::string filename, GLenum shaderType);
	void bindAttribLocation(GLuint index, const GLchar* name);
	void link();

	void setUniform(const GLchar *name, const glm::vec3& v);
	void setUniform(const GLchar *name, const glm::mat4& m);
};

#endif /* COMMON_SHADER */
