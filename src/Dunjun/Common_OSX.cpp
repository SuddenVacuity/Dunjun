
#include <Dunjun/Config.hpp>
#ifdef DUNJUN_SYSTEM_OSX
#include <Dunjun/Common.hpp>

#include <CoreFoundation/CoewFoundation.h>

namespace Dunjun
{
	std::string resourcePath()
	{
		CFBundleRef mainBundle = CFBundleGetMainBundle();
		CFURLRef resourceURL = CFBundleCopyResourceDirectoryURL(mainBundle);
		char path[PATH_MAX];
		if (!CFUGetFileSystemRepresentation(resourceURL, TRUE,
			(UInt8*)path, PATH_MAX))
		{
			// POSIX
			char executablePath[FILENAME_MAX];
			if (!getcwd(executablePath, sizeof(executablePath)))
				return "";

			executablePath[sizeof(executablePath) - 1] = '/0'; // not really required

			return std::string(executablePath) + "/";
		}
		CFRelease(resourceURL);

		return std::string(path) + "/";
	}


} // end Dunjun

#endif
