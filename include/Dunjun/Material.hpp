#ifndef DUNJUN_MATERIAL_HPP
#define DUNJUN_MATERIAL_HPP

#include <Dunjun/ShaderProgram.hpp>

namespace Dunjun
{
	struct Material
	{
		/*
			shaders
			
			diffuse
				map
				color

			specular
				map
				intensity(exponent)
				color

			// Other Things

			ambient occlusion
				map

			emissive light
				map

			detail
				map
				color

		*/

		Dunjun::ShaderProgram* shaders = nullptr;
		//Dunjun::Texture* texture;

		Dunjun::Texture* diffuseMap = nullptr;
		Color diffuseColor = ColorLib::White;

		Color specularColor = ColorLib::White;
		f32 specularExponent = 80.0f;
	};
} // end Dunjun

#endif
