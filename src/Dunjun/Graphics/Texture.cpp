
#include <Dunjun/Graphics/Texture.hpp>

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

	Texture::Texture(const Image& image,
		TextureFilter minMagFilter,
		TextureWrapMode wrapMode)
		: handle(0)
		, width((s32)image.width)
		, height((s32)image.height)
	{
		if (!loadFromImage(image, minMagFilter, wrapMode))
			throwRuntimeError("Could not create texture from image.");
	}

	Texture::~Texture()
	{
		if (handle)
			glDeleteTextures(1, &handle);
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
		if ((const ImageFormat&)image.format == ImageFormat::None )
			return false;

		width = image.width;
		height = image.height;

		if(!handle)
			glGenTextures(1, &handle);

		//glGenTextures(1, &m_object);
		glBindTexture(GL_TEXTURE_2D, handle);
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
					 getInternalFormat(image.format, true), 
					 width, height, 0, 
					 getInternalFormat(image.format, false), 
					 GL_UNSIGNED_BYTE, image.pixels);

		glBindTexture(GL_TEXTURE_2D, 0);
		
		return true;

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

		glBindTexture(GL_TEXTURE_2D, (tex && tex->handle ? tex->handle : 0));

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
} // End Namespace Dunjun
