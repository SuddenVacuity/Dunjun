#ifndef DUNJUN_COLOR_HPP
#define DUNJUN_COLOR_HPP

#include <Dunjun/Texture.hpp>

namespace Dunjun
{
	union Color // must come before render
	{
		u8 data[4];
		struct
		{
			u8 r, g, b, a;
		};
	};
	//
	//Color ratioColor(f32 red, f32 green, f32 blue, f32 lightness, f32 alpha)
	//{
	//	f32 num = red + blue + green;
	//
	//	f32 color[3] = { red, green, blue };
	//
	//	f32 l = lightness;
	//	f32 a = alpha;
	//
	//	// check values are valid
	//	if(l > 1.00000000f)
	//		l = 1.00000000f;
	//	else if (l < 0.00000000f)
	//		l = 0.00000000f;
	//
	//	if(a > 1.00000000f)
	//		a = 1.00000000f;
	//	else if (a < 0.00000000f)
	//		a = 0.00000000f;
	//
	//	 for (int i = 0; i < 3; i++)
	//		 if(color[i] < 0.00000000f)
	//			 color[i] = 0.00000000f;
	//
	//	// start making color
	//	if(num > 0)
	//	{
	//		color[0] = (red / num) * 255;
	//		color[1] = (green / num) * 255;
	//		color[2] = (blue / num) * 255;
	//	}		
	//	else
	//	{
	//		color[0] = 255;
	//		color[1] = 255;
	//		color[2] = 255;
	//	}
	//
	//	color[0] = (color[0] * lightness);
	//	color[1] = (color[1] * lightness);
	//	color[2] = (color[2] * lightness);
	//
	//	a = a * 255;
	//
	//	return {(u8)color[0], (u8)color[1], (u8)color[2], (u8)a};
	//}
	//

}

#endif
