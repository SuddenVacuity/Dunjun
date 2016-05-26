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
		Vector3 intensities = {1, 1, 1}; // color * brightness
		Vector3 ambientCoefficient = { 0.0001f, 0.0001f, 0.0001f };

		Attenuation attenuation;
	};



} // end Dunjun

#endif
