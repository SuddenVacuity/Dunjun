#ifndef DUNJUN_WINDOW_GLCONTEXTSETTINGS_HPP
#define DUNJUN_WINDOW_GLCONTEXTSETTINGS_HPP

#include <Dunjun/Window/VideoMode.hpp>

namespace Dunjun
{
	struct GLContextSettings
	{
		explicit GLContextSettings() = default;

		u32 depthBits = 0;
		u32 stencilBits = 0;
		u32 antialiasingLevel = 0;
		u32 anisotropicLevel = 0;

		u32 majorVersion = 2;
		u32 minorVersion = 1;
	};

} // end Dunjun

#endif
