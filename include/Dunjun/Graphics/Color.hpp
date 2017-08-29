#ifndef DUNJUN_GRAPHICS_COLOR_HPP
#define DUNJUN_GRAPHICS_COLOR_HPP

#include <Dunjun/Graphics/Texture.hpp>

namespace Dunjun
{
	GLOBAL const u32 COLOR_DEPTH = (u8)(-1);

	struct Color // must come before render
	{
		//u8 data[4];
		//struct
		//{
			u8 r, g, b, a;
		//};
		//
		//
		//Color(u8 r, u8 g, u8 b, u8 a = 0xFF)
		//	: r(r)
		//	, g(g)
		//	, b(b)
		//	, a(a)
		//{
		//}
		//
		//explicit Color(u32 rgba)
		//{
		//	r = (rgba >> 24) & 0xFF;
		//	g = (rgba >> 16) & 0xFF;
		//	b = (rgba >> 8) & 0xFF;
		//	a = (rgba >> 0) & 0xFF;
		//}
	};

	namespace ColorLib
	{
		GLOBAL const Vector3 BRIGHTNESS_COEFFICIENT = { 0.241f, 0.691f, 0.068f }; // default weight for calculating percieved brightness
		GLOBAL const Vector4 BRIGHTNESS_MAXVALUE = { COLOR_DEPTH * COLOR_DEPTH * BRIGHTNESS_COEFFICIENT.r,
													 COLOR_DEPTH * COLOR_DEPTH * BRIGHTNESS_COEFFICIENT.g,
													 COLOR_DEPTH * COLOR_DEPTH * BRIGHTNESS_COEFFICIENT.b,
													 Math::sqrt( COLOR_DEPTH * COLOR_DEPTH * BRIGHTNESS_COEFFICIENT.r +
																 COLOR_DEPTH * COLOR_DEPTH * BRIGHTNESS_COEFFICIENT.g +
																 COLOR_DEPTH * COLOR_DEPTH * BRIGHTNESS_COEFFICIENT.b ) };

		// returns u8 0-255 value
		//
		// Vector3 should be float value 0-1
		// Color should be ColorLib::Color
		u8 calculateBrightness(const Vector3& colorIntesities, const Vector3& weight = BRIGHTNESS_COEFFICIENT);

		// returns 0-255 value
		u8 calculateBrightness(const Color& color, const Vector3& weight = BRIGHTNESS_COEFFICIENT);

		// hue 0 = red, 1 = yellow, 2 = green, 3 = cyan, 4 = blue, 5 = magenta
		// saturation 0~1 -- 0 = grey, 1 = full color
		// lightness 0~1 -- 0 = black, 0.5 = normal, 1 = white
		Color HSLtoRGB(const f32& hue, const f32& saturation = 1.0f, const f32& lightness = 0.5f);

		Vector3 RGBtoHSL(const Color& color);

		// Mix two colors evenly.
		Color mix(Color c1, Color c2);

		// Mix two colors by ratio.
		// Ratio range: 0.0f, 1.0f
		// less effiecient than evenly mixing
		// use when many mixes would be needed to reach desired result
		Color mix(Color c1, Color c2, f32 ratio);

		Color removeChannelRed(const Color& color);

		Color removeChannelGreen(const Color& color);

		Color removeChannelBlue(const Color& color);

		Color greyScale(const f32& brightness);

	const Color White{0xFF, 0xFF, 0xFF, 0xFF};
	const Color Grey {0x7F, 0x7F, 0x7F, 0xFF};
	const Color Black{0x00, 0x00, 0x00, 0xFF};

	const Color Red   {0xFF, 0x00, 0x00, 0xFF};
	const Color Green {0x00, 0xFF, 0x00, 0xFF};
	const Color Blue  {0x00, 0x00, 0xFF, 0xFF};

	const Color Yellow  {0xFF, 0xFF, 0x00, 0xFF};
	const Color Magenta {0xFF, 0x00, 0xFF, 0xFF};
	const Color Cyan	{0x00, 0xFF, 0xFF, 0xFF};

	const Color Orange = mix(Red, Yellow);	   // 0xFF7F00 FF
	const Color Rose   = mix(Red, Magenta);	   // 0xFF007F FF
	const Color Acid   = mix(Green, Yellow);   // 0x7FFF00 FF
	const Color Spring = mix(Green, Cyan);	   // 0x00FF7F FF
	const Color Purple = mix(Blue, Magenta);   // 0x7F00FF FF
	const Color Azure  = mix(Blue, Cyan);	   // 0x007FFF FF
											   // 
	const Color Pink  = mix(Red, White);	   // 0xFF7F7F FF
	const Color Mint  = mix(Green, White);	   // 0x7FFF7F FF
	const Color Berry = mix(Blue, White);	   // 0x7F7FFF FF
											   // 
	const Color Brown = mix(Orange, Black);	   // 0x7F4000 FF
	} // end ColorLib
} // end Dunjun

#endif
