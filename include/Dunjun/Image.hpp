#ifndef DUNJUN_IMAGE_HPP
#define DUNJUN_IMAGE_HPP

// Image Loader


// Define functions are variables to be used in Image.cpp and main.cpp

#include <Dunjun/TickCounter.hpp>


namespace Dunjun
{
	enum ImageFormat // Format is the list of image types to check
	{
		None = 0,
		Greyscale = 1, // image type then number of bytes per pixel
		GreyscaleAlpha = 2,
		RGB = 3,
		RGBA = 4,
	};
	class Image // create an image class
	{
	public:

		Image(); // declair Image function
		Image(u32 width, u32 height, ImageFormat format, const u8* pixels = nullptr); // and declair its variables
		Image(const Image& other); // the image function will be for half the image this part is the other half
		Image& operator=(const Image& other);
		~Image(); // 

		bool loadFromFile(const std::string& filename); // get the name of the file to be loaded
		bool loadFromImage(u32 width, u32 height, ImageFormat format, const u8* pixels); // load info from memory

		const u8* getPixels() const;
		u8* getPixel(u32 column, u32 row) const; // make function and vairables to get a pixel from a specific spot
		void setPixel(u32 column, u32 row, const u32* pixel); // make function to set a pixel to a specific spot

		void flipVertically(); // declair flip function
//		void rotate90CCW();

//		void copyRectangleFromImage(const Image& src, 
//									uInt32 srcCol, uInt32 srcRow,
//									uInt32 destCol, uInt32 destRow,
//									uInt32 width, uInt32 height);

		// declair variables that will be used in cpp files
		//ReadOnly<ImageFormat, Image> format; // image format
		//ReadOnly<u32, Image> width; // texture width
		//ReadOnly<u32, Image> height; // texture height
		//ReadOnly<u8*, Image> pixels; // 8 bit pixel array

		ImageFormat getFormat() const;

		u32 getWidth() const;
		u32 getHeight() const;

	private:
		ImageFormat m_format;
		u32 m_width;
		u32 m_height;
		u8* m_pixels;
	};
}


#endif
