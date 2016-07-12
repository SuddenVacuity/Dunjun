#ifndef DUNJUN_GRAPHICS_COLOR_HPP
#define DUNJUN_GRAPHICS_COLOR_HPP

#include <Dunjun/Graphics/RenderTexture.hpp>

namespace Dunjun
{
	struct Color // must come before render
	{
		u8 data[4];
		struct
		{
			u8 r, g, b, a;
		};

		GLOBAL const u32 COLOR_DEPTH = 256;

		Color(u8 r, u8 g, u8 b, u8 a = 0xFF)
			: r(r)
			, g(g)
			, b(b)
			, a(a)
		{
		}

		explicit Color(u32 rgba)
		{
			r = (rgba >> 24) & 0xFF;
			g = (rgba >> 16) & 0xFF;
			b = (rgba >> 8) & 0xFF;
			a = (rgba >> 0) & 0xFF;
		}
	};

	namespace ColorLib
	{
		namespace
		{
		// returns u8 0-255 value
		//
		// Vector3 should be float value 0-1
		// Color should be ColorLib::Color
		u8 calculateBrightness(const Vector3& colorIntesities, const Vector3& weight = {0.241f, 0.691f, 0.068f})
		{
			// if all are 0 return 0
			if(colorIntesities.r == 0 && colorIntesities.g == 0 && colorIntesities.b == 0)
				return 0;

			return Math::sqrt((((colorIntesities.r * Color::COLOR_DEPTH) * (colorIntesities.r * Color::COLOR_DEPTH)) * weight.r) +
							  (((colorIntesities.g * Color::COLOR_DEPTH) * (colorIntesities.g * Color::COLOR_DEPTH)) * weight.g) +
							  (((colorIntesities.b * Color::COLOR_DEPTH) * (colorIntesities.b * Color::COLOR_DEPTH)) * weight.b)) - 1;
		}

		// returns 0-255 value
		u8 calculateBrightness(const Color& color, const Vector3& weight = {0.241f, 0.691f, 0.068f})
		{
			if(color.r == 0 && color.g == 0 && color.b == 0)
				return 0.0f;

			return Math::sqrt(((color.r * color.r) * weight.r) +
							  ((color.g * color.g) * weight.g) +
							  ((color.b * color.b) * weight.b));
		}
		// Mix two colors evenly.
		Color mix(Color c1, Color c2)
		{
			return{ static_cast<u8>((c1.r + c2.r) / 2),
					static_cast<u8>((c1.g + c2.g) / 2),
					static_cast<u8>((c1.b + c2.b) / 2),
					static_cast<u8>((c1.a + c2.a) / 2) };
		}

		// Mix two colors by ratio.
		// Ratio range: 0.0f, 1.0f
		// less effiecient than evenly mixing
		// use when many mixes would be needed to reach desired result
		Color mix(Color c1, Color c2, f32 ratio)
		{
			assert(!(ratio < Constants::ZERO || ratio > Constants::ONE));

			f32 r2 = 1.0f - ratio;

			return{ static_cast<u8>((ratio * c1.r + r2 * c2.r) / 2),
					static_cast<u8>((ratio * c1.g + r2 * c2.g) / 2),
					static_cast<u8>((ratio * c1.b + r2 * c2.b) / 2),
					static_cast<u8>((ratio * c1.a + r2 * c2.a) / 2) };
		}

		Color removeChannelRed(const Color& color)
		{
			Color c = color;

			// TODO: make it possible to divide removed color unevenly between other colors
			c.g += c.r / 2;
			c.b += c.r / 2;

			c.r = 0;

			u8 biggest = c.r;

			if (c.g > c.r)
				biggest = c.g;
			if (c.b > c.g)
				biggest = c.b;

			if (c.r != 0)
				c.r = (biggest / c.r) * (Color::COLOR_DEPTH - (biggest + 1));
														
			if (c.g != 0)								
				c.g = (biggest / c.g) * (Color::COLOR_DEPTH - (biggest + 1));
													
			if (c.b != 0)							
				c.b = (biggest / c.b) * (Color::COLOR_DEPTH - (biggest + 1));

			return Color(static_cast<u8>(c.r),
						 static_cast<u8>(c.g),
						 static_cast<u8>(c.b));
		}

		Color removeChannelGreen(const Color& color)
		{
			Color c = color;

			// TODO: make it possible to divide removed color unevenly between other colors
			c.r += c.g / 2;
			c.b += c.g / 2;

			c.g = 0;

			u8 biggest = c.r;

			if (c.g > c.r)
				biggest = c.g;
			if (c.b > c.g)
				biggest = c.b;

			if (c.r != 0)
				c.r = (biggest / c.r) * (Color::COLOR_DEPTH - (biggest + 1));

			if (c.g != 0)
				c.g = (biggest / c.g) * (Color::COLOR_DEPTH - (biggest + 1));

			if (c.b != 0)
				c.b = (biggest / c.b) * (Color::COLOR_DEPTH - (biggest + 1));

			return Color(static_cast<u8>(c.r),
						 static_cast<u8>(c.g),
						 static_cast<u8>(c.b));
		}

		Color removeChannelBlue(const Color& color)
		{
			Color c = color;

			// TODO: make it possible to divide removed color unevenly between other colors
			c.r += c.b / 2;
			c.g += c.b / 2;

			c.b = 0;

			u8 biggest = c.r;

			if (c.g > c.r)
				biggest = c.g;
			if (c.b > c.g)
				biggest = c.b;

			if (c.r != 0)
				c.r = (biggest / c.r) * (Color::COLOR_DEPTH - (biggest + 1));

			if (c.g != 0)
				c.g = (biggest / c.g) * (Color::COLOR_DEPTH - (biggest + 1));

			if (c.b != 0)
				c.b = (biggest / c.b) * (Color::COLOR_DEPTH - (biggest + 1));

			return Color(static_cast<u8>(c.r),
						 static_cast<u8>(c.g),
						 static_cast<u8>(c.b));
		}

		Color greyScale(const f32& brightness)
		{
			Color c = Color(0xFF, 0xFF, 0xFF);

			c.r = brightness;
			c.g = brightness;
			c.b = brightness;
				    
			return c;
		}

		Color greyScale(const Color& color)
		{
			f32 brightness = calculateBrightness(color);
			Color c = Color(0xFF, 0xFF, 0xFF);

			c.r = brightness;
			c.g = brightness;
			c.b = brightness;

			return c;
		}

	const Color White(0xFFFFFFFF);
	const Color Grey (0x7F7F7FFF);
	const Color Black(0x000000FF);

	const Color Red   (0xFF0000FF);
	const Color Green (0x00FF00FF);
	const Color Blue  (0x0000FFFF);

	const Color Yellow  (0xFFFF00FF);
	const Color Magenta (0xFF00FFFF);
	const Color Cyan	(0x00FFFFFF);

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
	} // end anon namespace
	} // end ColorLib
} // end Dunjun

#endif
