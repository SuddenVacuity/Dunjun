#ifndef DUNJUN_GRAPHICS_TEXTURE_HPP
#define DUNJUN_GRAPHICS_TEXTURE_HPP

// this file is a wrapper for textures

#include <Dunjun/Graphics/Image.hpp>

namespace Dunjun
{
	//enum to make it obvious that its a filter
	enum class TextureFilter : s32
	{
		Linear = GL_LINEAR,
		Nearest = GL_NEAREST,
	};

	//enum to make it obvious that its a wrap mode
	enum class TextureWrapMode : s32
	{
		Clamp = GL_CLAMP,
		Repeat = GL_REPEAT,
		ClampToEdge = GL_CLAMP_TO_EDGE,
		ClampToBorder = GL_CLAMP_TO_BORDER,
		MirroedRepeat = GL_MIRRORED_REPEAT,
	};

	struct Texture // create texture class
	{
	public:
		u32 handle;
		s32 width;
		s32 height;

		GLOBAL void bind(const Texture* tex, u32 position);
	};

	void destroyTexture(Texture& texture);

	Texture loadTextureFromFile(const char* filename,
								TextureFilter minMagFilter = TextureFilter::Linear,
								TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);

	Texture loadTextureFromImage(const Image& image,
								 TextureFilter minMagFilter = TextureFilter::Linear,
								 TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);
}


#endif
