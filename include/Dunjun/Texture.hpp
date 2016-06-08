#ifndef DUNJUN_TEXTURE_HPP
#define DUNJUN_TEXTURE_HPP

// this file is a wrapper for textures

#include <Dunjun/Image.hpp>

namespace Dunjun
{
	//enum to make it obvious that its a filter
	enum class TextureFilter : GLint
	{
		Linear = GL_LINEAR,
		Nearest = GL_NEAREST,
	};

	//enum to make it obvious that its a wrap mode
	enum class TextureWrapMode : GLint
	{
		Clamp = GL_CLAMP,
		Repeat = GL_REPEAT,
		ClampToEdge = GL_CLAMP_TO_EDGE,
		ClampToBorder = GL_CLAMP_TO_BORDER,
		MirroedRepeat = GL_MIRRORED_REPEAT,
	};

	class Texture : public NonCopyable // create texture class
	{
	public:
		Texture();
		Texture(const Image& image,
			TextureFilter minMagFilter = TextureFilter::Linear,
			TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);

		bool loadFromFile(const std::string& filename,
			TextureFilter minMagFilter = TextureFilter::Linear,
			TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);

		bool loadFromImage(const Image& image,
			TextureFilter minMagFilter = TextureFilter::Linear,
			TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);

		virtual ~Texture();

		GLOBAL void bind(const Texture* tex, GLuint position);
		//GLOBAL void unbindTexture(GLuint position);
		
		//GLsizei width;
		//GLsizei height;
		s32 getWidth() const;
		s32 getHeight() const;

		GLuint getNativeHandle() const;

	private:
		friend class RenderTexture;
		friend class GBuffer;

		s32 m_width;
		s32 m_height;

		GLuint m_object;
	};
}


#endif
