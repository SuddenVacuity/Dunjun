
#include <Dunjun/GBuffer.hpp>

namespace Dunjun
{
	GBuffer::GBuffer()
	{
		glGenFramebuffersEXT(1, &fbo);
	}

	GBuffer::~GBuffer()
	{
		glDeleteFramebuffersEXT(1, &fbo);
	}


	bool GBuffer::create(u32 w, u32 h)
	{
		if (w == width.data && h == height.data)
			return true;

		width = w;
		height = h;

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo.data);

		GLuint depthRenderBuffer;

		glGenRenderbuffersEXT(1, &depthRenderBuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthRenderBuffer);

		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,
			GL_DEPTH_COMPONENT,
			(GLsizei)width, (GLsizei)height);

		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
			GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER_EXT,
			depthRenderBuffer);

		std::vector<GLenum> drawBuffers;

		auto addRT = [&drawBuffers, w, h](Texture& tex, 
										  GLenum attachment, 
										  GLint internalFormat, 
										  GLenum format, 
										  GLenum type)
		{
			glBindTexture(GL_TEXTURE_2D, (GLuint)tex.m_object);

			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
				(GLsizei)w, (GLsizei)h,
				0, format, type, 0);

			tex.width = w;
			tex.height = h;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT,
				attachment,
				tex.m_object, 0);

			drawBuffers.push_back(attachment);
		};

		addRT(diffuse,	GL_COLOR_ATTACHMENT0_EXT, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
		addRT(specular, GL_COLOR_ATTACHMENT1_EXT, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
		addRT(world,	GL_COLOR_ATTACHMENT2_EXT, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
		addRT(depth,	GL_DEPTH_ATTACHMENT_EXT, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT);

		glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);

		if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
			return false;

		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
		return true;
	}


	void GBuffer::bind(const GBuffer* b)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, b != nullptr ? b->fbo : 0);
	}

	void GBuffer::unbind(const GBuffer* b)
	{
		glFlush();
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}

} // end Dunjun
