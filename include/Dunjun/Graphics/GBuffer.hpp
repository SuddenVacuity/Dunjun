#ifndef DUNJUN_GRAPHICS_GBUFFER_HPP
#define DUNJUN_GRAPHICS_GBUFFER_HPP

#include <Dunjun/Graphics/Mesh.hpp>

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
		u32 fbo;

		Texture textures[TextureType::Count];

	};
		bool createGBuffer(GBuffer& b, u32 w, u32 h);
		void destroyGBuffer(GBuffer& b);

		void bindGBuffer(const GBuffer* b);
} // end Dunjun

#endif
