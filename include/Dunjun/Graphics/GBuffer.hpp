#ifndef DUNJUN_GRAPHICS_GBUFFER_HPP
#define DUNJUN_GRAPHICS_GBUFFER_HPP

#include <Dunjun/Graphics/Texture.hpp>

namespace Dunjun
{
	struct GBuffer
	{
	public:
		enum TextureType
		{
			Diffuse,
			Specular,
			Normal,
			Depth,

			Count,
		};

		u32 width;
		u32 height;

		Texture textures[TextureType::Count];

		u32 fbo;

		//virtual ~GBuffer();

		bool create(u32 w, u32 h);
		void destroy();

		GLOBAL void bind(const GBuffer* b);
	};
} // end Dunjun

#endif
