#ifndef DUNJUN_RENDER_TEXTURE_HPP
#define DUNJUN_RENDER_TEXTURE_HPP

#include <Dunjun/Texture.hpp>

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
		};

		RenderTexture();
		virtual ~RenderTexture();

		bool create(u32 w, u32 h, TextureType t,
			TextureFilter minMagFilter = TextureFilter::Linear,
			TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);

		void setActive(bool active = true);
		void flush();

		void bindTexture(TextureType t, GLuint position) const;

		Texture colorTexture;
		Texture depthTexture;

		ReadOnly<u32, RenderTexture> width;
		ReadOnly<u32, RenderTexture> height;
		ReadOnly<TextureType, RenderTexture> type;
		ReadOnly<GLuint, RenderTexture> fbo; //frame buffer object
		
	};


} // end Dunjun

#endif
