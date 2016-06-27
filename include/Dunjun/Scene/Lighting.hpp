#ifndef DUNJUN_SCENE_LIGHTING_HPP 
#define DUNJUN_SCENE_LIGHTING_HPP 

#include <Dunjun/Graphics/ModelAsset.hpp>

namespace Dunjun
{
	struct Attenuation
	{
		f32 constant = 1.0f;
		f32 linear = 0.00f;
		f32 quadratic = 1.0f;
	};

	struct BaseLight
	{
		Color color = ColorLib::White;
		f32 intensity = 1.0f; // color intensity = max(color component) * brightness
		f32 brightness = 1.0f; // perceived brightness
		Vector3 colorIntensity = Vector3(1, 1, 1);
	};

	struct DirectionalLight : BaseLight
	{
		Vector3 direction;
	};

	// TODO: fix light fall off: it doens't fall off at the same rate on y axis as on the others
	struct PointLight : BaseLight
	{
		Vector3 position = {0, 0, 0};
		Attenuation attenuation;
		mutable f32 range = 16.0f;
	};

	inline Vector3 calculateLightIntensities(Color color, f32 strength)
	{
		//intensity = strength;

		Vector3 c(color.r, color.g, color.b);

		return (c / (Color::COLOR_DEPTH - 1)) * strength;
	}

	inline f32 calculateLightBrightness(const Vector3& color, const Vector3& weight = Vector3(0.241f, 0.691f, 0.068f))
	{
		f32 r = (color.r * color.r) * weight.r;
		f32 g = (color.g * color.g) * weight.g;
		f32 b = (color.b * color.b) * weight.b;

		return Math::sqrt(r + b + g);
	}

	inline f32 calculateLightRange(f32 intensity, Color color, Attenuation attenuation)
	{
		f32 i = intensity * (f32)std::max(color.r, std::max(color.g, color.b));

		f32 r = -attenuation.linear +
				Math::sqrt(attenuation.linear * attenuation.linear -
						   4.0f * attenuation.quadratic * 
						   (attenuation.constant - i));
		r /= 2.0f * attenuation.quadratic;

		return r;
	}

	struct SpotLight : PointLight
	{
		Radian coneAngle = Radian(0.35f);
		Vector3 direction = {0, 0, -1};
	};


} // end Dunjun

#endif
