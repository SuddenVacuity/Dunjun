#ifndef DUNJUN_SYSTEM_FILESYSTEM_HPP
#define DUNJUN_SYSTEM_FILESYSTEM_HPP

#include <Dunjun/System/Array.hpp>

namespace Dunjun
{
	namespace FileSystem
	{
		std::string getBasePath();
		std::string getPreferencesPath(const std::string organization, const std::string application);
		std::string getFileDirectory(const std::string& filepath);
	} // end FileSystem
} // end Dunjun

#endif
