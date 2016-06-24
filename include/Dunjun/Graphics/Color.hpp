#ifndef DUNJUN_GRAPHICS_COLOR_HPP
#define DUNJUN_GRAPHICS_COLOR_HPP

#include <Dunjun/Graphics/RenderTexture.hpp>

namespace Dunjun
{
	struct Color // must come before render
	{
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

		u8 data[4];
		struct
		{
			u8 r, g, b, a;
		};
	};

	namespace ColorLib
	{
		namespace
		{
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
