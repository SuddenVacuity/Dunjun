#ifndef DUNJUN_SCENE_LIGHTING_HPP 
#define DUNJUN_SCENE_LIGHTING_HPP 

#include <Dunjun/ModelAsset.hpp>

namespace Dunjun
{
	struct Attenuation
	{
		f32 constant = 1.0f;
		f32 linear = 0.00f;
		f32 quadratic = 1.0f;
	};

	struct PointLight
	{
		Vector3 position = {0, 0, 0};
		Color color = defaultWhite;
		f32 brightness = 1.0f; // intensity = max(color component) * brightness

		Attenuation attenuation;

		f32 range = 16.0f;

		inline void calculateRange()
		{
			f32 i = brightness * (f32)std::max(color.r, std::max(color.g, color.b));

			f32 r = -attenuation.linear +
				Math::sqrt(attenuation.linear * attenuation.linear -
					4.0f * attenuation.quadratic * (attenuation.constant - i));
			r /= 2.0f * attenuation.quadratic;

			range = r;
		}
	};



} // end Dunjun

#endif
