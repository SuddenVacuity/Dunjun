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

	struct Texture : public NonCopyable // create texture class
	{
	public:
		s32 width = 0;
		s32 height = 0;
		u32 handle = 0;

		Texture() = default;
		Texture(const Image& image,
			TextureFilter minMagFilter = TextureFilter::Linear,
			TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);

		virtual ~Texture();

		bool loadFromFile(const std::string& filename,
			TextureFilter minMagFilter = TextureFilter::Linear,
			TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);

		bool loadFromImage(const Image& image,
			TextureFilter minMagFilter = TextureFilter::Linear,
			TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);


		GLOBAL void bind(const Texture* tex, u32 position);
	};
}


#endif
