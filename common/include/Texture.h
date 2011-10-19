#ifndef COMMON_TEXTURE
#define COMMON_TEXTURE

#include <string>

namespace Common
{
	class Texture
	{
	public:
		Texture(const char * filename);
		virtual ~Texture();

		void bind() const;
		void unbind() const;

		inline const int width() const { return m_width; }
		inline const int height() const { return m_height; }

	private:
		int m_width;
		int m_height;

		unsigned m_texID;
	};
}

#endif /* COMMON_TEXTURE */