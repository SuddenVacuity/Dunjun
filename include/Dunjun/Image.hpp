#ifndef DUNJUN_IMAGE_HPP
#define DUNJUN_IMAGE_HPP

// Image Loader


// Define functions are variables to be used in Image.cpp and main.cpp

#include <Dunjun/Types.hpp>


namespace Dunjun
{
	class Image // create an image class
	{
	public:
		enum Format // Format is the list of image types to check
		{
			Format_Greyscale = 1, // image type then number of bytes per pixel
			Format_GreyscaleAlpha = 2,
			Format_RGB = 3,
			Format_RGBAlpha = 4,
		};

		Image(); // declair Image function
		Image(uInt32 width, uInt32 height, Format format, const uInt8* pixels = nullptr); // and declair its variables
		Image(const Image& other); // the image function will be for half the image this part is the other half
		Image& operator=(const Image& other);
		~Image(); // 

		bool loadFromFile(const char* filename); // get the name of the file to be loaded
		bool loadFromMemory(uInt32 width, uInt32 height, Format format, const uInt8* pixels); // load info from memory

		// have the function return the values to m_XXXXX to be used in cpp file
		inline uInt32 width() const { return m_width; }
		inline uInt32 height() const { return m_height; }
		inline Format format() const { return m_format; }
		inline uInt8* pixelPtr() const { return m_pixels; }

		uInt8* getPixel(uInt32 column, uInt32 row) const; // make function and vairables to get a pixel from a specific spot
		void setPixel(uInt32 column, uInt32 row, const uInt32* pixel); // make function to set a pixel to a specific spot

		void flipVertically(); // declair flip function
//		void rotate90CCW();

//		void copyRectangleFromImage(const Image& src, 
//									uInt32 srcCol, uInt32 srcRow,
//									uInt32 destCol, uInt32 destRow,
//									uInt32 width, uInt32 height);

		// declair variables that will be used in cpp files
		Format m_format; // image format
		uInt32 m_width; // texture width
		uInt32 m_height; // texture height
		uInt8* m_pixels; // 8 bit pixel array
	};
}


#endif