#ifndef DUNJUN_GBUFFER_HPP
#define DUNJUN_GBUFFER_HPP

#include <Dunjun/Texture.hpp>

namespace Dunjun
{
	class GBuffer
	{
	public:
		GBuffer();
		virtual ~GBuffer();

		bool create(u32 w, u32 h);

		GLOBAL void bind(const GBuffer* b);
		//GLOBAL void unbind(const GBuffer* b);

		ReadOnly<u32, GBuffer> width;
		ReadOnly<u32, GBuffer> height;

		ReadOnly<GLuint, GBuffer> fbo;

		Texture diffuse;   // diffuse color rgb
		Texture specular;  // specular color rbg + smoothness
		Texture normal;	   // world normals rgb

		Texture depth;
	};
} // end Dunjun

#endif
