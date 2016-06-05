#ifndef DUNJUN_RENDER_TEXTURE_HPP
#define DUNJUN_RENDER_TEXTURE_HPP

#include <Dunjun/GBuffer.hpp>

namespace Dunjun
{
	class RenderTexture
	{
	public:
		enum TextureType
		{
			Color = 1,
			Depth = 2,
			ColorAndDepth = Color | Depth,
			Light = 4 | Color,
		};

		RenderTexture();
		virtual ~RenderTexture();

		bool create(u32 w, u32 h,
			TextureType t = ColorAndDepth,
			TextureFilter minMagFilter = TextureFilter::Linear,
			TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);

		//void setActive(bool active = true);
		//void flush();

		GLOBAL void bind(const RenderTexture* rt);
		//GLOBAL void unbind(const RenderTexture* rt);

		//void bindTexture(TextureType t, GLuint position) const;

		Texture colorTexture;
		Texture depthTexture;

		ReadOnly<TextureType, RenderTexture> type;

		ReadOnly<u32, RenderTexture> width;
		ReadOnly<u32, RenderTexture> height;
		ReadOnly<GLuint, RenderTexture> fbo; //frame buffer object
	};


} // end Dunjun

#endif
