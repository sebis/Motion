#include "Texture.h"
#include "Utils.h"

#include <GL/glew.h>

namespace Common
{
	Texture::Texture(const char * filename)
	{
		char *data = 0;
		Utils::TextureInfo info;

		Utils::read_texture(filename, &data, &info);

		m_width = info.width;
		m_height = info.height;

		GLenum format = 0;
		if (info.pf == Utils::BGR)
			format = GL_BGR;
		else if (info.pf == Utils::BGRA)
			format = GL_BGRA;

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
		glGenTextures(1, &m_texID);
		glBindTexture(GL_TEXTURE_2D, m_texID);
	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);

		glBindTexture(GL_TEXTURE_2D, 0);
		delete [] data;
	}

	Texture::~Texture()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &m_texID);
	}

	void Texture::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_texID);
	}

	void Texture::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
