#ifndef DUNJUN_GRAPHICS_MATERIAL_HPP
#define DUNJUN_GRAPHICS_MATERIAL_HPP

#include <Dunjun/Graphics/ShaderProgram.hpp>

namespace Dunjun
{
	struct Material
	{
		enum class MaterialType : u32
		{
			Default,
		};

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
		MaterialType type = MaterialType::Default;

		Dunjun::Texture* diffuseMap = nullptr;
		Color diffuseColor = ColorLib::White;

		Color specularColor = ColorLib::White;
		f32 specularExponent = 80.0f;
	};
} // end Dunjun

#endif
