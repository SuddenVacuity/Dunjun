
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

	Texture loadTextureFromFile(const String& filename,
								TextureFilter minMagFilter,
								TextureWrapMode wrapMode)
	{
		Image image = loadImageFromFile(filename);
		defer(destroyImage(image));

		//if (!loadTextureFromFile(filename).handle)
		//	return false;
		flipVertically(image); // because textures load upside-down

		return loadTextureFromImage(image, minMagFilter, wrapMode);
	}

	Texture loadTextureFromImage(const Image& image,
								 TextureFilter minMagFilter,
								 TextureWrapMode wrapMode)
	{
		if (image.format == ImageFormat::None )
			return Texture{};

		Texture result = {0, image.width, image.height};

		//if(!result.handle)
		glGenTextures(1, &result.handle);

		//glGenTextures(1, &m_object);
		glBindTexture(GL_TEXTURE_2D, result.handle);
		defer(glBindTexture(GL_TEXTURE_2D, 0));

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
					 result.width, result.height, 0,
					 getInternalFormat(image.format, false), 
					 GL_UNSIGNED_BYTE, image.pixels);

		
		return result;

	}

	void destroyTexture(Texture& texture)
	{
		if (texture.handle)
			glDeleteTextures(1, &texture.handle);
	}

	void bindTexture(const Texture* tex, uSize_t position)
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
