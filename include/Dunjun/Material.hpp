#ifndef DUNJUN_MATERIAL_HPP
#define DUNJUN_MATERIAL_HPP

#include <Dunjun/ShaderProgram.hpp>

namespace Dunjun
{
	struct Material
	{
		Dunjun::ShaderProgram* shaders;
		Dunjun::Texture* texture;
	};
} // end Dunjun

#endif
