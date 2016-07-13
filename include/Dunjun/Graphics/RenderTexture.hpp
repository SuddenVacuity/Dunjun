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

		TextureType type = TextureType::Color;
		u32 fbo = 0;

		RenderTexture() = default;
		virtual ~RenderTexture();

		bool create(u32 w, u32 h,
			TextureType t = ColorAndDepth,
			TextureFilter minMagFilter = TextureFilter::Linear,
			TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);

		//void setActive(bool active = true);
		//void flush();

		GLOBAL void bind(const RenderTexture* rt);


	};	


} // end Dunjun

#endif
