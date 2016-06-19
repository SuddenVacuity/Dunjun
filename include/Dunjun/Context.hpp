#ifndef DUNJUN_CONTEXT_HPP
#define DUNJUN_CONTEXT_HPP

#include <Dunjun/ResourceHolder_objects.hpp>

namespace Dunjun
{
	struct Context
	{
		explicit Context(Window& window,
						 TextureHolder& textureHolder,
						 ShaderHolder& shaderHolder,
						 MeshHolder& meshHolder,
						 MaterialHolder& materialHolder)
			: window(window)
			, textureHolder(textureHolder)
			, shaderHolder(shaderHolder)
			, meshHolder(meshHolder)
			, materialHolder(materialHolder)
		{
		}

		Context(const Context&) = default;

		Window& window;

		TextureHolder& textureHolder;
		ShaderHolder& shaderHolder;
		MeshHolder& meshHolder;
		MaterialHolder& materialHolder;
	};
} // end Dunjun

#endif
