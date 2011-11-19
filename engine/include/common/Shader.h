#ifndef COMMON_SHADER
#define COMMON_SHADER

#include "Trace.h"

#include <string>
#include <map>
#include <GL/glew.h>

#include "glm/glm.hpp"

namespace Common
{
	class Shader
	{
	private:
		Shader();

		unsigned id_table[10];
		unsigned num_ids;

		unsigned p_id;

		bool load(std::string filename, GLenum shaderType);
		bool compile(unsigned id);
		void link();

		void bindAttribute(GLuint index, const GLchar* name);

	public:
		virtual ~Shader();

		enum ATTRIBUTE
		{
			POSITION = 0,
			COLOR = 1,
			NORMAL = 2,
			TEXCOORD = 3,
			POINTSIZE = 4,
		};

		void bind() const;
		void unbind() const;

		void setUniform(const GLchar *name, const glm::vec3& v);
		void setUniform(const GLchar *name, const glm::vec4& v);
		void setUniform(const GLchar *name, const glm::mat4& m);
		void setUniform(const GLchar *name, const glm::float_t& f);
		void setUniform(const GLchar *name, bool b);

		static Shader* find(const std::string& technique)
		{
			static std::map<std::string, Shader*> shaders;

			std::map<std::string, Shader*>::iterator it = shaders.find(technique);
			if (it != shaders.end())
				return it->second;

			Shader* shader = new Shader;

			// TODO: some better way to add resources per project but to find them globally
			static std::string path("resources/");

			if (!shader->load(path + technique + ".vert", GL_VERTEX_SHADER)) {
				Trace::error("Could not find %s.vert\n", (path + technique).c_str());
				return 0;
			}
			if (!shader->load(path + technique + ".frag", GL_FRAGMENT_SHADER)) {
				Trace::error("Could not find %s.frag\n", (path + technique).c_str());
				return 0;
			}
			if (shader->load(path + technique + ".geom", GL_GEOMETRY_SHADER)) {
				Trace::info("Geometry shader %s.geom loaded\n", technique.c_str());
			}

			GLenum err = glGetError();
			if (err != GL_NO_ERROR)
				Trace::error("OpenGL error: %d\n", err);

			shader->bindAttribute(POSITION, "in_Position");
			shader->bindAttribute(NORMAL, "in_Normal");
			shader->bindAttribute(COLOR, "in_Color");
			shader->bindAttribute(TEXCOORD, "in_TexCoord");
			shader->bindAttribute(POINTSIZE, "in_Size");

			err = glGetError();
			if (err != GL_NO_ERROR)
				Trace::error("OpenGL error: %d\n", err);

			shader->link();

			shaders[technique] = shader;
			return shader;
		}
	};
}

#endif /* COMMON_SHADER */
