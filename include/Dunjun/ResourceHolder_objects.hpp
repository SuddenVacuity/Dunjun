#ifndef DUNJUN_RESOURCE_HOLDER_OBJECTS_HPP
#define DUNJUN_RESOURCE_HOLDER_OBJECTS_HPP

#include <Dunjun/Scene/Lighting.hpp>

namespace Dunjun
{
	using TextureHolder = ResourceHolder<Texture, std::string>;
	using ShaderHolder = ResourceHolder<ShaderProgram, std::string>;

	extern TextureHolder g_textureHolder;
	extern ShaderHolder g_shaderHolder;
} // end Dunjun

#endif
