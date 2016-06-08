#ifndef DUNJUN_MODEL_ASSET_HPP
#define DUNJUN_MODEL_ASSET_HPP

#include <Dunjun/Material.hpp>

namespace Dunjun
{
	struct ModelAsset // includes the shader, the texture and the vbo's
	{
		Material* material = nullptr;
		const Mesh *mesh = nullptr;
	};
} // end Dunjun

#endif
