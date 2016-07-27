#ifndef DUNJUN_GRAPHICS_RENDERTEXTURE_HPP
#define DUNJUN_GRAPHICS_RENDERTEXTURE_HPP

#include <Dunjun/Graphics/GBuffer.hpp>

namespace Dunjun
{
	struct RenderTexture
	{
	public:
		enum TextureType
		{
			Color = 1,
			Depth = 2,
			ColorAndDepth = Color | Depth,
			Light = 4 | Color,
		};

		Texture colorTexture = {};
		Texture depthTexture = {};

		u32 width = 0;
		u32 height = 0;
		u32 fbo = 0;

		TextureType type = TextureType::Color;

	};

	bool createRenderTexture(RenderTexture& rt,
							 u32 width, u32 height,
							 RenderTexture::TextureType type = RenderTexture::ColorAndDepth,
							 TextureFilter minMagFilter = TextureFilter::Linear,
							 TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);

	void destroyRenderTexture(RenderTexture& rt);

	void bindRenderTexture(const RenderTexture* rt);




} // end Dunjun

#endif
