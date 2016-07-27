#ifndef DUNJUN_GRAPHICS_IMAGE_HPP
#define DUNJUN_GRAPHICS_IMAGE_HPP

// Image Loader


// Define functions are variables to be used in Image.cpp and main.cpp

#include <Dunjun/Window/Event.hpp>


namespace Dunjun
{
	enum class ImageFormat : u32// Format is the list of image types to check
	{
		None = 0,
		Greyscale = 1, // image type then number of bytes per pixel
		GreyscaleAlpha = 2,
		RGB = 3,
		RGBA = 4,
	};
	struct Image // create an image class
	{
	public:
		ImageFormat format;
		u32 width;
		u32 height;
		u8* pixels;
	};

	Image loadImageFromFile(const String& filename); // get the name of the file to be loaded
	Image loadImageFromMemory(u32 width, u32 height, ImageFormat format, const u8* pixels); // load info from memory

	void destroyImage(Image& image);

	u8* getPixel(u32 column, u32 row); // make function and vairables to get a pixel from a specific spot
	void setPixel(u32 column, u32 row, const u32* pixel); // make function to set a pixel to a specific spot

	void flipVertically(Image& image); // declair flip function
//		void rotate90CCW();

//		void copyRectangleFromImage(const Image& src, 
//									uInt32 srcCol, uInt32 srcRow,
//									uInt32 destCol, uInt32 destRow,
//									uInt32 width, uInt32 height);

}


#endif
