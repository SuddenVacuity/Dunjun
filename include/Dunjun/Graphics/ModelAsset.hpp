#ifndef DUNJUN_GRAPHICS_MODELASSET_HPP
#define DUNJUN_GRAPHICS_MODELASSET_HPP

#include <Dunjun/Graphics/Material.hpp>

namespace Dunjun
{
	struct ModelAsset // includes the shader, the texture and the vbo's
	{
		Material* material = nullptr;
		const Mesh *mesh = nullptr;
	};
} // end Dunjun

#endif
