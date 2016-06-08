
#include <Dunjun/Texture.hpp>

namespace Dunjun
{
	INTERNAL GLenum getInternalFormat(ImageFormat format, bool srgb)
	{
		switch (format)
		{
		case ImageFormat::Greyscale:
			return GL_LUMINANCE;
		case ImageFormat::GreyscaleAlpha:
			return GL_LUMINANCE_ALPHA;
		case ImageFormat::RGB:
			return (srgb ? GL_SRGB : GL_RGB);
		case ImageFormat::RGBA:
			return (srgb ? GL_SRGB_ALPHA : GL_RGBA);

		default:
		case ImageFormat::None:
			throw std::runtime_error("Non-valid ImageFormat.");
			return 0;
		}
	}

	Texture::Texture() // Initialize texture
		: m_handle(0)
		, m_width(0)
		, m_height(0)
	{
	}
	Texture::Texture(const Image& image,
		TextureFilter minMagFilter,
		TextureWrapMode wrapMode)
		: m_handle(0)
		, m_width((s32)image.getWidth())
		, m_height((s32)image.getHeight())
	{
		if (!loadFromImage(image, minMagFilter, wrapMode))
			throwRuntimeError("Could not create texture from image.");
	}

	bool Texture::loadFromFile(const std::string& filename,
		TextureFilter minMagFilter,
		TextureWrapMode wrapMode)
	{
		Image image;
		if (!image.loadFromFile(filename))
			return false;
		image.flipVertically(); // because textures load upside-down

		return loadFromImage(image, minMagFilter, wrapMode);
	}

	bool Texture::loadFromImage(const Image& image,
		TextureFilter minMagFilter,
		TextureWrapMode wrapMode)
	{
		if ((const ImageFormat&)image.getFormat() == ImageFormat::None )
			return false;

		m_width = image.getWidth();
		m_height = image.getHeight();

		if(!m_handle)
			glGenTextures(1, &m_handle);

		//glGenTextures(1, &m_object);
		glBindTexture(GL_TEXTURE_2D, m_handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<s32>(wrapMode)); // set the s axis (x) to repeat
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<s32>(wrapMode)); // set the t axis (y) to repeat
																	  //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL REPEAT); // set the r axis (z) to repeat

																	  /* set the border color for GL_CLAMP_TO_BORDER
																	  float color[] = {1.0f, 0.0f, 0.0f, 1.0f}; // create float array with the color in it
																	  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color); // apply the color to the border
																	  */

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<s32>(minMagFilter)); // set the texture min filter type
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<s32>(minMagFilter)); // set the texture max filter type
																		  // there are 3 types of texture filters
																		  // GL_NEAREST	keeps it as close as possible pixel to pixel
																		  // GL_LINEAR	blurs the pixels
																		  // GL_MIPMAP	blurs the pixel differently



	//	glTexImage2D(GL_TEXTURE_2D, 0, format, (GLsizei)m_width, (GLsizei)m_height, 0, image.getFormat(), GL_UNSIGNED_INT, image.getPixelPtr());
		glTexImage2D(GL_TEXTURE_2D, 0, 
					 getInternalFormat(image.getFormat(), true), 
					 m_width, m_height, 0, 
					 getInternalFormat(image.getFormat(), false), 
					 GL_UNSIGNED_BYTE, image.getPixels());

		glBindTexture(GL_TEXTURE_2D, 0);
		
		return true;

	}

	Texture::~Texture()
	{
		if(m_handle)
			glDeleteTextures(1, &m_handle);
	}

	void Texture::bind(const Texture* tex, u32 position)
	{
		if (position > 31)
		{
			std::cerr << "Texture can only be bound to postions [0 ... 31].\n";
			std::cerr << "Will bind to position 31." << std::endl;
			position = 31;
		}
		glActiveTexture(GL_TEXTURE0 + position);
		glClientActiveTexture(GL_TEXTURE0 + position);

		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, (tex && tex->m_handle ? tex->m_handle : 0));

		glDisable(GL_TEXTURE_2D);
	}

	//void Texture::unbindTexture(GLuint position)
	//{
	//	if (position> 31)
	//	{
	//		std::cerr << "Texture can only be bound to postions [0 ... 31].\n";
	//		std::cerr << "Will bind to position 31." << std::endl;
	//		position = 31;
	//	}
	//	glActiveTexture(GL_TEXTURE0 + position);
	//	glClientActiveTexture(GL_TEXTURE0 + position);
	//
	//	glEnable(GL_TEXTURE_2D);
	//
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//
	//	glDisable(GL_TEXTURE_2D);
	//
	//}

		s32 Texture::getWidth() const
		{
			return m_width;
		}

		s32 Texture::getHeight() const
		{
			return m_height;
		}

		GLuint Texture::getNativeHandle() const
		{
			return m_handle;
		}
} // End Namespace Dunjun
