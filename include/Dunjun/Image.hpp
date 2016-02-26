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
		Image(u32 width, u32 height, Format format, const u8* pixels = nullptr); // and declair its variables
		Image(const Image& other); // the image function will be for half the image this part is the other half
		Image& operator=(const Image& other);
		~Image(); // 

		bool loadFromFile(const char* filename); // get the name of the file to be loaded
		bool loadFromImage(u32 width, u32 height, Format format, const u8* pixels); // load info from memory

		// have the function return the values to m_XXXXX to be used in cpp file
		inline u32 getWidth() const { return m_width; }
		inline u32 getHeight() const { return m_height; }
		inline Format getFormat() const { return m_format; }
		inline u8* getPixelPtr() const { return m_pixels; }

		u8* getPixel(u32 column, u32 row) const; // make function and vairables to get a pixel from a specific spot
		void setPixel(u32 column, u32 row, const u32* pixel); // make function to set a pixel to a specific spot

		void flipVertically(); // declair flip function
//		void rotate90CCW();

//		void copyRectangleFromImage(const Image& src, 
//									uInt32 srcCol, uInt32 srcRow,
//									uInt32 destCol, uInt32 destRow,
//									uInt32 width, uInt32 height);

	private:
		// declair variables that will be used in cpp files
		Format m_format; // image format
		u32 m_width; // texture width
		u32 m_height; // texture height
		u8* m_pixels; // 8 bit pixel array
	};
}


#endif
