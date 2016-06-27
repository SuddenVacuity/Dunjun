#ifndef  DUNJUN_WINDOW_VIDEOMODE_HPP
#define  DUNJUN_WINDOW_VIDEOMODE_HPP

#include <Dunjun/System/OpenGL_SDL.hpp>

namespace Dunjun
{
	struct VideoMode
	{
		u32 width = 0;
		u32 height = 0;
		u32 bitsPerPixel = 24;

		VideoMode();
		VideoMode(u32 modeWidth, u32 modeHeight, u32 modeBitsPerPixel = 24);

		GLOBAL VideoMode getDesktopMode();
		GLOBAL const std::vector<VideoMode>& getFullscreenModes();

		bool isValid() const;
	};

	bool operator==(const VideoMode& left, const VideoMode& right);
	bool operator!=(const VideoMode& left, const VideoMode& right);

	bool operator>(const VideoMode& left, const VideoMode& right);
	bool operator<(const VideoMode& left, const VideoMode& right);

	bool operator>=(const VideoMode& left, const VideoMode& right);
	bool operator<=(const VideoMode& left, const VideoMode& right);

} // end Dunjun

#endif
