#include <Dunjun/Graphics/Image.hpp>

#define STB_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

//#include <iostream> // included in Common.hpp

namespace Dunjun
{

	Image loadImageFromFile(const std::string& filename) // load info from a file
	{
		Image result;

		int w, h, f; // declair variables to receive
		u8* p = stbi_load(filename.c_str(), &w, &h, &f, 0); // load them using stb library
		defer(stbi_image_free(p)); // free pixels using stb library

			if (!p) // if there are no pixels error
				{
					std::cerr << stbi_failure_reason() << std::endl;
					return {};
				}
	
		return loadImageFromMemory(w, h, (ImageFormat)f, p); // load info from file to memory
	}

	Image loadImageFromMemory(u32 w, u32 h, ImageFormat f, const u8* p) // start organizing image info
	{
		if (w == 0) // width error
		{
			std::cerr << "Zero width image." << std::endl;
			return {};
		}
		if (h == 0) // height error
		{
			std::cerr << "Zero height image." << std::endl;
			return {};
		}

		Image result = {};

		// move info to m_XXXXX
		result.width = w;
		result.height = h;
		result.format = f;
			
		size_t imageSize = result.width * result.height * (size_t)result.format; // find total number of pixels in the image

		//Allocator& a = defaultAllocator();

		if(result.pixels)
			delete[] result.pixels;
			//a.deallocate(img.pixels);

		result.pixels = new u8[imageSize];

		if(p != nullptr)
			memcpy(result.pixels, p, imageSize);

		//if (result.pixels) // delete any pixels that exist already
		//	delete[] result.pixels;
		//
		//result.pixels = new u8[imageSize]; // define m_pixels as total number of pixels
		//
		//if (p != nullptr)
		//	std::memcpy(result.pixels, p, imageSize); // copy image size from pixels to m_pixels

		return result;
	}

	void destroyImage(Image& image) // destructor
	{
		if (image.pixels) // check if there are pixels
			delete[] image.pixels;
			//defaultAllocator().deallocate(image.pixels); // if so delete them
	}

	u8* getPixel(u32 column, u32 row)
	{
		Image result;

		if (column >= result.width || row >= result.height)
			return nullptr; // end when image is done being read
		return result.pixels + (row * result.width + column) * (size_t)result.format; // get pixels info
	}
	void setPixel(u32 column, u32 row, const u32* pixel)
	{
		Image result;

		if (column >= result.width || row >= result.height)
			return; // end when image is done being set

		u8* p = getPixel(column, row); // make temp variable for getPixel() values
		std::memcpy(p, pixel, (size_t)result.format); // does this have a use?
	}

	void flipVertically(Image& image)
	{
		//Allocator& a = defaultAllocator();

		std::size_t pitch = image.width * (size_t)image.format; // get the size of the info to be flipped
		u32 halfrows = image.height / 2; // divide the image into 2 halves, upper and lower

		u8* rowBuffer = new u8[pitch]; // create a new pointer for the buffer the size of pitch
		//defer(a.deallocate(rowBuffer));

		for (u32 i = 0; i < halfrows; i++) // generate the image until all halfrows are done i is used as a counter
		{
			u8* row = image.pixels + (i * image.width) * (size_t)image.format; // define row
			u8* oppositeRow = image.pixels + ((image.height - i - 1) * image.width) * (size_t)image.format;

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

} // end Dunjun
