#ifndef DUNJUN_SYSTEM_FILESYSTEM_HPP
#define DUNJUN_SYSTEM_FILESYSTEM_HPP

#include <Dunjun/Common.hpp>

namespace Dunjun
{
	namespace FileSystem
	{
		String getBasePath();
		String getPreferencesPath(const String& organization, const String& application);
		String getFileDirectory(const String& filepath);
	} // end FileSystem
} // end Dunjun

#endif
