
#include <Dunjun/Graphics/RenderTexture.hpp>

namespace Dunjun
{
	void destroyRenderTexture(RenderTexture& rt)
	{
		if(rt.fbo)
			glDeleteFramebuffersEXT(1, &rt.fbo);

		destroyTexture(rt.colorTexture);
		destroyTexture(rt.depthTexture);
	}

	bool createRenderTexture(RenderTexture& rt, 
							 u32 w, u32 h, 
							 RenderTexture::TextureType t, 
							 TextureFilter minMagFilter, 
							 TextureWrapMode wrapMode)
	{
		if(w == rt.width && h == rt.height && t == rt.type)
			return true;

		rt.type = t;
		rt.width = w;
		rt.height = h;
		
		//if(!rt.fbo)
			glGenFramebuffersEXT(1, &rt.fbo);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, rt.fbo);
		defer(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, rt.fbo));

		defer(glBindTexture(GL_TEXTURE_2D, 0));

		//GLuint depthRenderBuffer = 0;
		//
		//glGenRenderbuffersEXT(1, &depthRenderBuffer);
		//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthRenderBuffer);
		//
		//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, 
		//						 GL_DEPTH_COMPONENT, 
		//						 (GLsizei)width, (GLsizei)height);
		//
		//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, 
		//							 GL_DEPTH_ATTACHMENT, 
		//							 GL_RENDERBUFFER_EXT, 
		//							 depthRenderBuffer);

		if (rt.type & RenderTexture::TextureType::Color)
		{
			if(!rt.colorTexture.handle)
				glGenTextures(1, &rt.colorTexture.handle);

			glBindTexture(GL_TEXTURE_2D, (u32)rt.colorTexture.handle);

			// light
			if(rt.type & RenderTexture::TextureType::Light)
			{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F,
						(s32)rt.width, (s32)rt.height,
						0, GL_RGBA, GL_FLOAT, 0);
			}
			else
			{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 
						 (s32)rt.width, (s32)rt.height,
						 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			}


			rt.colorTexture.width = rt.width;
			rt.colorTexture.height = rt.height;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)minMagFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)minMagFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)wrapMode);

			glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, 
									GL_COLOR_ATTACHMENT0_EXT, 
									rt.colorTexture.handle, 0);
		}
		if (rt.type & RenderTexture::TextureType::Depth)
		{
			if (!rt.depthTexture.handle)
				glGenTextures(1, &rt.depthTexture.handle);

			glBindTexture(GL_TEXTURE_2D, (u32)rt.depthTexture.handle);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 
						(s32)rt.width, (s32)rt.height,
						0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

			rt.depthTexture.width = rt.width;
			rt.depthTexture.height = rt.height;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)minMagFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)minMagFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)wrapMode);

			glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT,
				GL_DEPTH_ATTACHMENT_EXT,
				rt.depthTexture.handle, 0);
		}

		u32 drawBuffersLength = 0;
		GLenum drawBuffers[2];

		if (rt.type & RenderTexture::TextureType::Color || rt.type & RenderTexture::TextureType::Light)
			drawBuffers[drawBuffersLength++] = GL_COLOR_ATTACHMENT0_EXT;
		if (rt.type & RenderTexture::TextureType::Depth)
			drawBuffers[drawBuffersLength++] = GL_DEPTH_ATTACHMENT_EXT;
		

		glDrawBuffers(drawBuffersLength, &drawBuffers[0]);

		if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT )
		{
			//glBindTexture(GL_TEXTURE_2D, 0);
			//glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
			return false;

		}
		//glBindTexture(GL_TEXTURE_2D, 0);
		//glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
		return true;

	}

	void bindRenderTexture(const RenderTexture* rt)
	{
		if(!rt)
		{
			glFlush();
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		}
		else
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, rt->fbo);
		}

		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, rt != nullptr ? rt->fbo : 0);
	}
} // end Dunjun
