#include <Dunjun/Image.hpp>

#define STB_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

//#include <iostream> // included in Common.hpp

namespace Dunjun
{
	Image::Image()
		: format((ImageFormat)0) // set default values for m_XXXXX files
		, width(0)
		, height (0)
		, pixels (nullptr)
	{

	}
	Image::Image(u32 w, u32 h, ImageFormat f, const u8* p) // 1st half of the image
		: format((ImageFormat)0) // set default values for m_XXXXX files
		, width(0)
		, height (0)
		, pixels (nullptr)
	{
		loadFromImage(w, h, f, p); // sets variables to the m_XXXXX values
	}
	Image::Image(const Image& other) // 2nd half of the image
		: format((ImageFormat)0) // set default values for m_XXXXX files
		, width(0)
		, height(0)
		, pixels(nullptr)
	{
		loadFromImage(other.width, other.height, other.format, other.pixels); // create varialbles other.m_XXXXX values
	}
	Image& Image::operator=(const Image& other) //
	{
		loadFromImage(other.width, other.height, other.format, other.pixels); // return the other.m_XXXXX values
		return *this;
	}
	Image::~Image() // destructor
	{
		if (pixels) // check if there are pixels
			delete[] pixels; // if so delete them
	}

	bool Image::loadFromFile(const std::string& filename) // load info from a file
	{
		int w, h, f; // declair variables to receive
		u8* p = stbi_load(filename.c_str(), &w, &h, &f, 0); // load them using stb library

			if (!p) // if there are no pixels error
				{
					std::cerr << stbi_failure_reason() << std::endl;
					return false;
				}
	
			loadFromImage(w, h, (ImageFormat)f, p); // load info from file to memory

			stbi_image_free(p); // free pixels using stb library

			if (pixels) // check for m_pixels
				return true;
			return false;
	
	}
	bool Image::loadFromImage(u32 w, u32 h, ImageFormat f, const u8* p) // start organizing image info
	{
		if (w == 0) // width error
		{
			std::cerr << "Zero width image." << std::endl;
			return false;
		}
		if (h == 0) // height error
		{
			std::cerr << "Zero height image." << std::endl;
			return false;
		}
		if (f <= 0 || format >= 4) // format error
		{
			std::cerr << "Invalid texture format." << std::endl;
			return false;
		}

		// if no errors do this
		else // move info to m_XXXXX
			width = w;
			height = h;
			format = f;

			size_t imageSize = width * height * (size_t)format; // find total number of pixels in the image

			if (pixels) // delete any pixels that exist already
				delete[] pixels;

			pixels = new u8[imageSize]; // define m_pixels as total number of pixels

			if (p != nullptr)
				std::memcpy(pixels, p, imageSize); // copy image size from pixels to m_pixels

		return true;
	}

	u8* Image::getPixel(u32 column, u32 row) const
	{
		if (column >= width || row >= height)
			return nullptr; // end when image is done being read
		return pixels + (row * width + column) * (size_t)format.data; // get pixels info
	}
	void Image::setPixel(u32 column, u32 row, const u32* pixel)
	{
		if (column >= width || row >= height)
			return; // end when image is done being set

		u8* p = getPixel(column, row); // make temp variable for getPixel() values
		std::memcpy(p, pixel, (size_t)format.data); // does this have a use?
	}

	void Image::flipVertically()
	{
		std::size_t pitch = width * (size_t)format.data; // get the size of the info to be flipped
		u32 halfrows = height / 2; // divide the image into 2 halves, upper and lower
		u8* rowBuffer = new u8[pitch]; // create a new pointer for the buffer the size of pitch

		for (u32 i = 0; i < halfrows; i++) // generate the image until all halfrows are done i is used as a counter
		{
			u8* row = pixels + (i * width) * (size_t)format.data; // define row
			u8* oppositeRow = pixels + ((height - i - 1) * width) * (size_t)format.data;

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
