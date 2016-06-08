
#include <Dunjun/RenderTexture.hpp>

namespace Dunjun
{
	RenderTexture::RenderTexture()
		: colorTexture()
		, depthTexture()
		, m_width(0)
		, m_height(0)
		, m_type(TextureType::Color)
		, m_fbo(0)
	{
	}

	RenderTexture::~RenderTexture()
	{
		if(m_fbo)
			glDeleteFramebuffersEXT(1, &m_fbo);
	}

	bool RenderTexture::create(u32 w, u32 h, TextureType t, TextureFilter minMagFilter, TextureWrapMode wrapMode)
	{
		if(w == m_width && h == m_height && t == m_type)
			return true;

		m_type = t;
		m_width = w;
		m_height = h;
		
		if(!m_fbo)
			glGenFramebuffersEXT(1, &m_fbo);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

		//GLuint depthRenderBuffer = 0;
		//
		//glGenRenderbuffersEXT(1, &depthRenderBuffer);
		//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthRenderBuffer);
		//
		//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, 
		//						 GL_DEPTH_COMPONENT, 
		//						 (GLsizei)m_width, (GLsizei)m_height);
		//
		//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, 
		//							 GL_DEPTH_ATTACHMENT, 
		//							 GL_RENDERBUFFER_EXT, 
		//							 depthRenderBuffer);

		if (m_type & TextureType::Color)
		{
			if(!colorTexture.m_handle)
				glGenTextures(1, &colorTexture.m_handle);

			glBindTexture(GL_TEXTURE_2D, (u32)colorTexture.m_handle);

			// light
			if(m_type & TextureType::Light)
			{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F,
						(GLsizei)m_width, (GLsizei)m_height,
						0, GL_RGBA, GL_FLOAT, 0);
			}
			else
			{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 
						 (GLsizei)m_width, (GLsizei)m_height,
						 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			}


			colorTexture.m_width = m_width;
			colorTexture.m_height = m_height;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)minMagFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)minMagFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)wrapMode);

			glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, 
									GL_COLOR_ATTACHMENT0_EXT, 
									colorTexture.m_handle, 0);
		}
		if (m_type & TextureType::Depth)
		{
			if (!depthTexture.m_handle)
				glGenTextures(1, &depthTexture.m_handle);

			glBindTexture(GL_TEXTURE_2D, (u32)depthTexture.m_handle);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 
						(GLsizei)m_width, (GLsizei)m_height,
						0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

			depthTexture.m_width = m_width;
			depthTexture.m_height = m_height;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)minMagFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)minMagFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)wrapMode);

			glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT,
				GL_DEPTH_ATTACHMENT_EXT,
				depthTexture.m_handle, 0);
		}

		std::vector<GLenum> drawBuffers;

		if (m_type & TextureType::Color || m_type & TextureType::Light)
			drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0_EXT);
		if (m_type & TextureType::Depth)
			drawBuffers.emplace_back(GL_DEPTH_ATTACHMENT_EXT);
		

		glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);

		if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT )
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
			return false;

		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
		return true;

	}

	void RenderTexture::bind(const RenderTexture* rt)
	{
		if(!rt)
			glFlush();

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, rt != nullptr ? rt->m_fbo : 0);
	}

	//void RenderTexture::unbind(const RenderTexture* rt)
	//{
	//	glFlush();
	//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//}
	
	//void RenderTexture::bindTexture(TextureType t, GLuint position) const
	//{
	//	if (position > 31)
	//	{
	//		std::cerr << "Texture can only be bound to postions [0 ... 31].\n";
	//		std::cerr << "Will bind to position 31." << std::endl;
	//		position = 31;
	//	}
	//
	//	if(colorTexture.m_object && type == Color)
	//		Texture::bind(&colorTexture, position);
	//	else if (colorTexture.m_object && type == Depth)
	//		Texture::bind(&depthTexture, position);
	//	else if (colorTexture.m_object && colorTexture.m_object && type == ColorAndDepth)
	//		Texture::bind(&depthTexture, position);
	//	else
	//		Texture::bind(nullptr, position);
	//}

	u32 RenderTexture::getWidth() const
	{
		return m_width;
	}

	u32 RenderTexture::getHeight() const
	{
		return m_height;
	}

	RenderTexture::TextureType RenderTexture::getType() const
	{
		return m_type;
	}

	u32 RenderTexture::getNativeHandle() const
	{
		return m_fbo;
	}
} // end Dunjun
