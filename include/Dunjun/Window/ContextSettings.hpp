#ifndef DUNJUN_WINDOW_CONTEXTSETTINGS_HPP
#define DUNJUN_WINDOW_CONTEXTSETTINGS_HPP

#include <Dunjun/Window/VideoMode.hpp>

namespace Dunjun
{
	struct ContextSettings
	{
		explicit ContextSettings() = default;

		u32 depthBits = 0;
		u32 stencilBits = 0;
		u32 antialiasingLevel = 0;
		u32 anisotropicLevel = 0;

		u32 majorVersion = 2;
		u32 minorVersion = 1;
	};

} // end Dunjun

#endif
