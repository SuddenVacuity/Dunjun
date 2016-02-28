
#include <Dunjun/Texture.hpp>
#include <Dunjun/Common.hpp>

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
		: m_object(0)
		, m_width(0)
		, m_height(0)
	{
		glGenTextures(1, &m_object);
	}
	Texture::Texture(const Image& image,
		GLint minMagFilter,
		GLint wrapMode)
		: m_object(0)
		, m_width(image.getWidth())
		, m_height(image.getHeight())
	{
		if (!loadFromImage(image, minMagFilter, wrapMode))
			throw std::runtime_error("Could not create texture from image.");
	}

	bool Texture::loadFromFile(const char* filename,
		GLint minMagFilter,
		GLint wrapMode)
	{
		Image image;
		if (!image.loadFromFile(filename))
			return false;
		image.flipVertically(); // because textures load upside-down

		return loadFromImage(image, minMagFilter, wrapMode);
	}

	bool Texture::loadFromImage(const Image& image,
		GLint minMagFilter,
		GLint wrapMode)
	{
		if (image.getFormat() <= 0 || image.getFormat() > 4 )
			return false;

		m_width = (GLfloat)image.getWidth();
		m_height = (GLfloat)image.getHeight();

		glGenTextures(1, &m_object);
		glBindTexture(GL_TEXTURE_2D, m_object);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode); // set the s axis (x) to repeat
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode); // set the t axis (y) to repeat
																	  //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL REPEAT); // set the r axis (z) to repeat

																	  /* set the border color for GL_CLAMP_TO_BORDER
																	  float color[] = {1.0f, 0.0f, 0.0f, 1.0f}; // create float array with the color in it
																	  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color); // apply the color to the border
																	  */

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMagFilter); // set the texture min filter type
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMagFilter); // set the texture max filter type
																		  // there are 3 types of texture filters
																		  // GL_NEAREST	keeps it as close as possible pixel to pixel
																		  // GL_LINEAR	blurs the pixels
																		  // GL_MIPMAP	blurs the pixel differently



	//	glTexImage2D(GL_TEXTURE_2D, 0, format, (GLsizei)m_width, (GLsizei)m_height, 0, image.getFormat(), GL_UNSIGNED_INT, image.getPixelPtr());
		glTexImage2D(GL_TEXTURE_2D, 0, getInternalFormat(image.getFormat(), true), (GLsizei)m_width, (GLsizei)m_height, 0, getInternalFormat(image.getFormat(), false), GL_UNSIGNED_BYTE, image.getPixelPtr());

		glBindTexture(GL_TEXTURE_2D, 0);
		
		return true;

	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_object);
	}

	void Texture::bind(GLuint position)
	{
		if (position> 31)
		{
			std::cerr << "Texture can only be bound to postions [0 ... 31].\n";
			std::cerr << "Will bind to position 31." << std::endl;
			position = 31;
		}
		glActiveTexture(GL_TEXTURE0 + position);
		glClientActiveTexture(GL_TEXTURE0 + position);

		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, (m_object ? m_object : 0));

		glDisable(GL_TEXTURE_2D);
	}
} // End Namespace Dunjun
