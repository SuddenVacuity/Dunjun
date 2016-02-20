#ifndef DUNJUN_TEXTURE_HPP
#define DUNJUN_TEXTURE_HPP

// this file is a wrapper for textures

#include <Dunjun/NonCopyable.hpp>
#include <Dunjun/Image.hpp>
#include <Dunjun/OpenGL.hpp>

namespace Dunjun
{
	class Texture : public NonCopyable // create texture class
	{
	public:
		Texture();
		Texture(const Image& image,
				GLint minMagFilter = GL_LINEAR,
				GLint wrapMode = GL_CLAMP_TO_EDGE);

		bool loadFromFile(const char* filename,
						  GLint minMagFilter = GL_LINEAR,
						  GLint wrapMode = GL_CLAMP_TO_EDGE);

		bool loadFromImage(const Image& image,
							GLint minMagFilter = GL_LINEAR,
							GLint wrapMode = GL_CLAMP_TO_EDGE);

		virtual ~Texture();

		void bind(GLuint position);

		inline GLuint getObject() const { return m_object; }
		inline GLfloat getWidth() const { return m_width; }
		inline GLfloat getHeight() const { return m_height; }
	private:
		GLuint m_object;
		GLfloat m_width;
		GLfloat m_height;
	};
}


#endif