
#include <Dunjun/Graphics/GBuffer.hpp>

namespace Dunjun
{
	void destroyGBuffer(GBuffer& b)
	{
		if(b.fbo)
			glDeleteFramebuffersEXT(1, &b.fbo);
	}


	bool createGBuffer(GBuffer& b, u32 w, u32 h)
	{
		if (w == b.width && h == b.height)
			return true;

		b.width = w;
		b.height = h;

		if(!b.fbo)
			glGenFramebuffersEXT(1, &b.fbo);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, b.fbo);
		defer(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0));
		

		//GLuint depthRenderBuffer = 0;
		//
		//glGenRenderbuffersEXT(1, &depthRenderBuffer);
		//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthRenderBuffer);
		//
		//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,
		//	GL_DEPTH_COMPONENT,
		//	(GLsizei)width, (GLsizei)height);
		//
		//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
		//	GL_DEPTH_ATTACHMENT,
		//	GL_RENDERBUFFER_EXT,
		//	depthRenderBuffer);

		Array<GLenum> drawBuffers = defaultAllocator();

		glEnable(GL_TEXTURE_2D);
		defer(glBindTexture(GL_TEXTURE_2D, 0));

		/* begin lambda */
		auto addRT = [&drawBuffers, w, h](Texture& tex, 
										  GLenum attachment, 
										  s32 internalFormat, 
										  GLenum format, 
										  GLenum type)
		{
			//if(!tex.handle)
				glGenTextures(1, &tex.handle);

			glBindTexture(GL_TEXTURE_2D, (u32)tex.handle);

			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
						(s32)w, (s32)h,
						0, format, type, nullptr);

			tex.width = w;
			tex.height = h;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glFramebufferTexture2D(GL_FRAMEBUFFER,
				attachment,
				GL_TEXTURE_2D,
				tex.handle, 0);

			if(attachment != GL_DEPTH_ATTACHMENT)
				append(drawBuffers, attachment);
		};/* end lambda */	

		addRT(b.textures[GBuffer::Diffuse],	 GL_COLOR_ATTACHMENT0, GL_RGB8,				 GL_RGB ,			 GL_UNSIGNED_BYTE);
		addRT(b.textures[GBuffer::Specular], GL_COLOR_ATTACHMENT1, GL_RGBA8,			 GL_RGBA,			 GL_UNSIGNED_BYTE);
		addRT(b.textures[GBuffer::Normal],	 GL_COLOR_ATTACHMENT2, GL_RGB10_A2,			 GL_RGBA,			 GL_FLOAT);
		addRT(b.textures[GBuffer::Depth],	 GL_DEPTH_ATTACHMENT , GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT);

		glDrawBuffers(len(drawBuffers), &drawBuffers[0]);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			//glBindTexture(GL_TEXTURE_2D, 0);
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return false;

		}
		//glBindTexture(GL_TEXTURE_2D, 0);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return true;
	}

	void bindGBuffer(const GBuffer* b)
	{
		glCheckError();

		if(!b)
		{
			glCheckError();

			glFlush();
			glCheckError();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glCheckError();

			glCheckError();

		}
		else
		{
			glCheckError();

			glBindFramebuffer(GL_FRAMEBUFFER, b->fbo);
			glCheckError();

			glCheckError();

		}

		//glBindFramebuffer(GL_FRAMEBUFFER, b != nullptr ? b->fbo : 0);
	}
} // end Dunjun
