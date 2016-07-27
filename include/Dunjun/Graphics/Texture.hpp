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

		//GLOBAL void bind(const Texture* tex, u32 position);
	};


	Texture loadTextureFromFile(const String& filename,
								TextureFilter minMagFilter = TextureFilter::Linear,
								TextureWrapMode wrapMode = TextureWrapMode::Repeat);

	Texture loadTextureFromImage(const Image& image,
								 TextureFilter minMagFilter = TextureFilter::Linear,
								 TextureWrapMode wrapMode = TextureWrapMode::Repeat);


	void destroyTexture(Texture& texture);
	void bindTexture(const Texture* tex, uSize_t position);

} // end Dunjun


#endif
