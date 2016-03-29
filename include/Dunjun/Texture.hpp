#ifndef DUNJUN_TEXTURE_HPP
#define DUNJUN_TEXTURE_HPP

// this file is a wrapper for textures

#include <Dunjun/Image.hpp>

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
		
		ReadOnly<GLuint, Texture> object;
		ReadOnly<GLsizei, Texture> width;
		ReadOnly<GLsizei, Texture> height;
	};
}


#endif
