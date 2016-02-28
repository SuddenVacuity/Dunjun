#include <Dunjun/Image.hpp>

#define STB_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

namespace Dunjun
{
	Image::Image()
		: m_format((ImageFormat)0) // set default values for m_XXXXX files
		, m_width(0)
		, m_height (0)
		, m_pixels (nullptr)
	{

	}
	Image::Image(u32 width, u32 height, ImageFormat format, const u8* pixels) // 1st half of the image
		: m_format((ImageFormat)0) // set default values for m_XXXXX files
		, m_width(0)
		, m_height (0)
		, m_pixels (nullptr)
	{
		loadFromImage(width, height, format, pixels); // sets variables to the m_XXXXX values
	}
	Image::Image(const Image& other) // 2nd half of the image
		: m_format((ImageFormat)0) // set default values for m_XXXXX files
		, m_width(0)
		, m_height(0)
		, m_pixels(nullptr)
	{
		loadFromImage(other.m_width, other.m_height, other.m_format, other.m_pixels); // create varialbles other.m_XXXXX values
	}
	Image& Image::operator=(const Image& other) //
	{
		loadFromImage(other.m_width, other.m_height, other.m_format, other.m_pixels); // return the other.m_XXXXX values
		return *this;
	}
	Image::~Image() // destructor
	{
		if (m_pixels) // check if there are pixels
			delete[] m_pixels; // if so delete them
	}

	bool Image::loadFromFile(const char* filename) // load info from a file
	{
		int width, height, format; // declair variables to receive
		u8* pixels = stbi_load(filename, &width, &height, &format, 0); // load them using stb library

			if (!pixels) // if there are no pixels error
				{
					std::cerr << stbi_failure_reason() << std::endl;
					return false;
				}
	
			loadFromImage(width, height, (ImageFormat)format, pixels); // load info from file to memory

			stbi_image_free(pixels); // free pixels using stb library

			if (m_pixels) // check for m_pixels
				return true;
			return false;
	
	}
	bool Image::loadFromImage(u32 width, u32 height, ImageFormat format, const u8* pixels) // start organizing image info
	{
		if (width == 0) // width error
		{
			std::cerr << "Zero width image." << std::endl;
			return false;
		}
		if (height == 0) // height error
		{
			std::cerr << "Zero height image." << std::endl;
			return false;
		}
		if (format <= 0 || format >= 4) // format error
		{
			std::cerr << "Invalid texture format." << std::endl;
			return false;
		}

		// if no errors do this
		else // move info to m_XXXXX
			m_width = width;
			m_height = height;
			m_format = format;

			std::size_t imageSize = width * height * format; // find total number of pixels in the image

			if (m_pixels) // delete any pixels that exist already
				delete[] m_pixels;

			m_pixels = new u8[imageSize]; // define m_pixels as total number of pixels

			if (pixels != nullptr)
				std::memcpy(m_pixels, pixels, imageSize); // copy image size from pixels to m_pixels

		return true;
	}

	u8* Image::getPixel(u32 column, u32 row) const
	{
		if (column >= m_width || row >= m_height)
			return nullptr; // end when image is done being read
		return m_pixels + (row * m_width + column) * m_format; // get pixels info
	}
	void Image::setPixel(u32 column, u32 row, const u32* pixel)
	{
		if (column >= m_width || row >= m_height)
			return; // end when image is done being set

		u8* p = getPixel(column, row); // make temp variable for getPixel() values
		std::memcpy(p, pixel, m_format); // does this have a use?
	}

	void Image::flipVertically()
	{
		std::size_t pitch = m_width * m_format; // get the size of the info to be flipped
		u32 halfrows = m_height / 2; // divide the image into 2 halves, upper and lower
		u8* rowBuffer = new u8[pitch]; // create a new pointer for the buffer the size of pitch

		for (u32 i = 0; i < halfrows; i++) // generate the image until all halfrows are done i is used as a counter
		{
			u8* row = m_pixels + (i * m_width) * m_format; // define row
			u8* oppositeRow = m_pixels + ((m_height - i - 1) * m_width) * m_format;

			std::memcpy(rowBuffer, row, pitch); // moves info to rowBuffer from row. pitch is the amount of info
			std::memcpy(row, oppositeRow, pitch);
			std::memcpy(oppositeRow, rowBuffer, pitch);
		}

		delete[] rowBuffer;

	}
//	void Image::rotate90CCW()
//	{
//
//	}
//
//	void Image::copyRectangleFromImage(const Image& src,
//										uInt32 srcCol, uInt32 srcRow,
//										uInt32 destCol, uInt32 destRow,
//										uInt32 width, uInt32 height)
//	{
//
//	}

}
