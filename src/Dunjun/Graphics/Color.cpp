
#include <Dunjun/Graphics/Color.hpp>

namespace Dunjun
{
	namespace ColorLib
	{
			// returns u8 0-255 value
			//
			// Vector3 should be float value 0-1
			// Color should be ColorLib::Color
			u8 calculateBrightness(const Vector3& colorIntesities, const Vector3& weight)
			{
				// if all are 0 return 0
				if (colorIntesities.r == 0 && colorIntesities.g == 0 && colorIntesities.b == 0)
					return 0;

				return Math::sqrt((((colorIntesities.r * COLOR_DEPTH) * (colorIntesities.r * COLOR_DEPTH)) * weight.r) +
					(((colorIntesities.g * COLOR_DEPTH) * (colorIntesities.g * COLOR_DEPTH)) * weight.g) +
					(((colorIntesities.b * COLOR_DEPTH) * (colorIntesities.b * COLOR_DEPTH)) * weight.b));
			}

			// returns 0-255 value
			u8 calculateBrightness(const Color& color, const Vector3& weight)
			{
				if (color.r + color.g + color.b == 0)
					return 0;

				return Math::sqrt(((color.r * color.r) * weight.r) +
					((color.g * color.g) * weight.g) +
					((color.b * color.b) * weight.b));
			}

			// hue 0 = red, 1 = yellow, 2 = green, 3 = cyan, 4 = blue, 5 = magenta
			// saturation 0~1 -- 0 = grey, 1 = full color
			// lightness 0~1 -- 0 = black, 0.5 = normal, 1 = white
			Color HSLtoRGB(const f32& hue, const f32& saturation, const f32& lightness)
			{
				f32 bitDepth = COLOR_DEPTH;
				Color result = { 0, 0, 0, bitDepth };

				logPrint(g_loggerGraphics, "HSLtoRGB() START: Input {%f, %f, %f}",
											hue, saturation, lightness);

				//if (lightness <= 0.0f)
				//{
				//	logPrint(g_loggerGraphics, "HSLtoRGB() END: Lightness is 0.0f: Output {%d, %d, %d, %d}",
				//								result.r, result.g, result.b, result.a);
				//	return result;
				//}
				//if (lightness >= 1.0f)
				//{
				//	result.r = bitDepth;
				//	result.g = bitDepth;
				//	result.b = bitDepth;
				//	logPrint(g_loggerGraphics, "HSLtoRGB() END: Lightness is 1.0f: Output {%d, %d, %d, %d}",
				//								result.r, result.g, result.b, result.a);
				//	return result;
				//}

				////////////////////////////
				// hue
				//
				// turn float into integer
				u8 range = static_cast<u8>(hue);

				// subtract integer from float to get scaler then: bitDepth * scaler
				const u8 scaledColor = bitDepth * (hue - (f32)(range));

				// mod by number of colors
				range = range % 6;

				switch (range)
				{
				default: break;
					// if max color = COLOR_DEPTH
					// if increasing color = scaledColor
					// if decreasing color = COLOR_DEPTH - scaledColor
				case 0: result.r = bitDepth;				result.g = scaledColor;	 logPrint(g_loggerGraphics, "HSLtoRGB() range: red >> yellow");	break; // red to yellow
				case 1: result.r = bitDepth - scaledColor;	result.g = bitDepth;	 logPrint(g_loggerGraphics, "HSLtoRGB() range: yellow >> green");	break; // yellow to green
				case 2:	result.g = bitDepth;				result.b = scaledColor;	 logPrint(g_loggerGraphics, "HSLtoRGB() range: green >> cyan");	break; // green to cyan
				case 3:	result.g = bitDepth - scaledColor;	result.b = bitDepth;	 logPrint(g_loggerGraphics, "HSLtoRGB() range: cyan >> blue");	break; // cyan to blue
				case 4: result.b = bitDepth;				result.r = scaledColor;	 logPrint(g_loggerGraphics, "HSLtoRGB() range: blue >> magenta");	break; // blue to magenta
				case 5: result.b = bitDepth - scaledColor;	result.r = bitDepth;	 logPrint(g_loggerGraphics, "HSLtoRGB() range: magenta >> red");	break; // magenta to red
				}

				///////////////////////////////
				// saturation
				//
				// brightness is what the color is at 0.0f saturation
				//const u8 brightness = calculateBrightness(result);
				const f32 brightness = bitDepth * (f32)(result.r + result.g + result.b) / (bitDepth * 3.0f);

				if (saturation <= 0.0f)
				{
					result.r = brightness;
					result.g = brightness;
					result.b = brightness;
					
					logPrint(g_loggerGraphics, "HSLtoRGB() saturation is 0.0f: Result {%d, %d, %d, %d}",
												result.r, result.g, result.b, result.a);
				}
				else if (saturation < 1.0f)
				{
					// TODO: HSVtoRGB that keeps even percieved brightness among hues
					result.r = brightness + saturation * (f32)((f32)result.r - brightness);
					result.g = brightness + saturation * (f32)((f32)result.g - brightness);
					result.b = brightness + saturation * (f32)((f32)result.b - brightness);
					
					logPrint(g_loggerGraphics, "HSLtoRGB() saturation is %f: Result {%d, %d, %d, %d}",
												saturation, result.r, result.g, result.b, result.a);
				}
				else
					logPrint(g_loggerGraphics, "HSLtoRGB() saturation is 1.0f: Result {%d, %d, %d, %d}",
												result.r, result.g, result.b, result.a);

				/////////////////////////////////
				// lightness
				// split into if(increasing) else if(decreasing)
				//if(saturation != 0)
				if (lightness > 0.5f)
				{
					// turn 0.5~1.0 into 0.0~1.0
					const f32 upperLight = (lightness - 0.5f) * 2;

					result.r = result.r + upperLight * (f32)(bitDepth - (f32)result.r);
					result.g = result.g + upperLight * (f32)(bitDepth - (f32)result.g);
					result.b = result.b + upperLight * (f32)(bitDepth - (f32)result.b);

					logPrint(g_loggerGraphics, "HSLtoRGB() lightness is %f: Result {%d, %d, %d, %d}",
												lightness, result.r, result.g, result.b, result.a);
				}
				else if (lightness < 0.5f)
				{
					// turn 0.0~0.5 into 0.0~1.0
					const f32 lowerLight = 2 * lightness;

					result.r = result.r * lowerLight;
					result.g = result.g * lowerLight;
					result.b = result.b * lowerLight;
					
					logPrint(g_loggerGraphics, "HSLtoRGB() lightness is %f: Result {%d, %d, %d, %d}",
												lightness, result.r, result.g, result.b, result.a);
				}

				logPrint(g_loggerGraphics, "HSLtoRGB() END: Output {%d, %d, %d, %d}",
											result.r, result.g, result.b, result.a);
				return result;
			}

			Vector3 RGBtoHSL(const Color& color)
			{
				f32 bitDepth = COLOR_DEPTH;

				f32 hue = 0;
				f32 saturation = 0;
				f32 lightness = 0;

				logPrint(g_loggerGraphics, "RGBtoHSL() START: Input {%d, %d, %d, %d}",
											color.r, color.g, color.b, color.a);

				//if (color.r == color.g && color.r == color.b)
				//{
				//	lightness = (f32)color.r / bitDepth;
				//	logPrint(g_loggerGraphics, "RGBtoHSL() END: Color values are equal: Output {%f, %f, %f}",
				//								hue, saturation, lightness);
				//	return{ hue, saturation, lightness };
				//}

				// value of the highest, lowest and variable color
				// are used for saturation and lightness
				f32 highest, variable, lowest;

				/////////////////////////////
				// find hue

				if (color.r == color.g && color.r == color.b)
				{
					logPrint(g_loggerGraphics, "RGBtoHSL() range: greyscale");
					highest = color.r;
					lowest = color.r;
					variable = color.r;
					hue = 0;
					logPrint(g_loggerGraphics, "RGBtoHSL() range values: highest{%f} lowest{%f} variable{%f} hue{%f}",
												highest, lowest, variable, hue);
				}
				else if (color.r > color.g && color.r > color.b)
				{
					highest = color.r;
					if (color.g >= color.b)
					{
						// red >> yellow
						logPrint(g_loggerGraphics, "RGBtoHSL() range: red >> yellow");
						variable = color.g;
						lowest = color.b;
						hue = 0 + (f32)color.g / bitDepth;
					}
					else
					{
						// magenta >> red
						logPrint(g_loggerGraphics, "RGBtoHSL() range: magenta >> red");
						variable = color.b;
						lowest = color.g;
						hue = 5 + (bitDepth - (f32)color.b) / bitDepth;
					}
					logPrint(g_loggerGraphics, "RGBtoHSL() range values: highest{%f} lowest{%f} variable{%f} hue{%f}",
												highest, lowest, variable, hue);
				}
				else if (color.g >= color.r && color.g > color.b)
				{
					highest = color.g;
					if (color.r >= color.b)
					{
						// yellow >> green
						logPrint(g_loggerGraphics, "RGBtoHSL() range: yellow >> green");
						variable = color.r;
						lowest = color.b;
						hue = 1 + (bitDepth - (f32)color.r) / bitDepth;
					}
					else
					{
						// green >> cyan
						logPrint(g_loggerGraphics, "RGBtoHSL() range: green >> cyan");
						variable = color.b;
						lowest = color.r;
						hue = 2 + (f32)color.b / bitDepth;
					}
					logPrint(g_loggerGraphics, "RGBtoHSL() range values: highest{%f} lowest{%f} variable{%f} hue{%f}",
												highest, lowest, variable, hue);
				}
				else if (color.b > color.r && color.b >= color.g)
				{
					highest = color.b;
					if (color.g >= color.r)
					{
						// cyan >> blue
						logPrint(g_loggerGraphics, "RGBtoHSL() range: cyan >> blue");
						variable = color.g;
						lowest = color.r;
						hue = 3 + (bitDepth - (f32)color.g) / bitDepth;
					}
					else
					{
						// blue >> magenta
						logPrint(g_loggerGraphics, "RGBtoHSL() range: blue >> magenta");
						variable = color.r;
						lowest = color.g;
						hue = 4 + (f32)color.r / bitDepth;
					}
					logPrint(g_loggerGraphics, "RGBtoHSL() range values: highest{%f} lowest{%f} variable{%f} hue{%f}",
						highest, lowest, variable, hue);
				}
				else
					logPrint(g_loggerError, "ColorLib::RBGtoHSL() hue value was not set.");

				///////////////////////////////
				// find lightness
				//u8 brightness = calculateBrightness(color);
				const f32 brightness = bitDepth * (f32)(color.r + color.g + color.b) / (bitDepth * 3.0f);

				f32 halfBitDepth = bitDepth / 2.0f;

				if (brightness > halfBitDepth)
				{
					lightness = 0.5f + (lowest / bitDepth) / 2.0f;
				}
				else if (brightness < halfBitDepth)
				{
					lightness = 0.5f - ((bitDepth - highest) / bitDepth) / 2.0f;
				}
				else
					lightness = 0.5f;

				/////////////////////////////
				// find saturation
				//if(lightness > 0.5f)
				//{
				//	// multiply by (bitDepth - brightness) / bitDepth to avoid a possible div by zero
				//	saturation = (f32)((highest - brightness) * (f32)(((bitDepth - brightness) / bitDepth)));
				//}
				//else
				//{
				//	// multiply by brightness / bitDepth to avoid a possible div by zero
				//	saturation = (f32)((brightness - lowest) * (f32)(brightness / bitDepth));
				//}

				f32 highDelta = highest - brightness;
				f32 lowDelta = brightness - lowest;

				if (highDelta > lowDelta)
				{
					if (brightness != bitDepth)
						saturation = highDelta / (bitDepth - brightness);
					else
						saturation = 0.0f;
				}
				else
				{
					if (brightness != 0.0f)
						saturation = lowDelta / brightness;
					else
						saturation = 0.0f;
				}

				logPrint(g_loggerGraphics, "RGBtoHSL() END: Output {%f, %f, %f}",
											hue, saturation, lightness);

				return{ hue, saturation, lightness };
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
					c.r = (biggest / c.r) * (COLOR_DEPTH - 1);

				if (c.g != 0)
					c.g = (biggest / c.g) * (COLOR_DEPTH - 1);

				if (c.b != 0)
					c.b = (biggest / c.b) * (COLOR_DEPTH - 1);

				return{ static_cast<u8>(c.r),
					static_cast<u8>(c.g),
					static_cast<u8>(c.b) };
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
					c.r = (biggest / c.r) * (COLOR_DEPTH - 1);

				if (c.g != 0)
					c.g = (biggest / c.g) * (COLOR_DEPTH - 1);

				if (c.b != 0)
					c.b = (biggest / c.b) * (COLOR_DEPTH - 1);

				return{ static_cast<u8>(c.r),
					static_cast<u8>(c.g),
					static_cast<u8>(c.b) };
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
					c.r = (biggest / c.r) * (COLOR_DEPTH - 1);

				if (c.g != 0)
					c.g = (biggest / c.g) * (COLOR_DEPTH - 1);

				if (c.b != 0)
					c.b = (biggest / c.b) * (COLOR_DEPTH - 1);

				return{ static_cast<u8>(c.r),
					static_cast<u8>(c.g),
					static_cast<u8>(c.b) };
			}

			Color greyScale(const f32& brightness)
			{
				Color c = Color{ 0xFF, 0xFF, 0xFF, 0xFF };

				c.r = brightness;
				c.g = brightness;
				c.b = brightness;

				return c;
			}

			Color greyScale(const Color& color)
			{
				f32 brightness = calculateBrightness(color);
				Color c = Color{ 0xFF, 0xFF, 0xFF, 0xFF };

				c.r = brightness;
				c.g = brightness;
				c.b = brightness;

				return c;
			}
	} // end ColorLib
} // end Dunjun

