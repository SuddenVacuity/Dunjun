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

		//ReadOnly<u32, GBuffer> width;
		//ReadOnly<u32, GBuffer> height;
		u32 getWidth() const;
		u32 getHeight() const;

		//ReadOnly<GLuint, GBuffer> fbo;
		GLuint getNativeHandle() const;

		Texture diffuse;   // diffuse color rgb
		Texture specular;  // specular color rbg + smoothness
		Texture normal;	   // world normals rgb

		Texture depth;

	private:
		u32 m_width;
		u32 m_height;

		GLuint m_fbo;
	};
} // end Dunjun

#endif
