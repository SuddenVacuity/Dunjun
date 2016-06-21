#ifndef DUNJUN_CONTEXT_HPP
#define DUNJUN_CONTEXT_HPP

#include <Dunjun/ResourceHolder_objects.hpp>

namespace Dunjun
{
	struct Context
	{
		Context() {}

		Context(Window& window,
						 TextureHolder& textureHolder,
						 ShaderHolder& shaderHolder,
						 MeshHolder& meshHolder,
						 MaterialHolder& materialHolder)
			: window(&window)
			, textureHolder(&textureHolder)
			, shaderHolder(&shaderHolder)
			, meshHolder(&meshHolder)
			, materialHolder(&materialHolder)
		{
		}

		Window* window = nullptr;

		TextureHolder* textureHolder = nullptr;
		ShaderHolder* shaderHolder = nullptr;
		MeshHolder* meshHolder = nullptr;
		MaterialHolder* materialHolder = nullptr;
	};
} // end Dunjun

#endif
