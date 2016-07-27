
#include <Dunjun/System/FileSystem.hpp>

#include <SDL/SDL_filesystem.h>

namespace Dunjun
{
	namespace FileSystem
	{
		String getBasePath()
		{
			return SDL_GetBasePath();
		}

		String getPreferencesPath(const String& organization, const String& application)
		{
			return SDL_GetPrefPath(cString(organization), cString(application));
		}

		String getFileDirectory(const String& filepath)
		{
			const sSize_t i = Strings::lastIndexAny(filepath, "/\\");
			if(i < 0)
				return filepath;
			return substring(filepath, 0, i);
		}

	} // end FileSystem
} // end Dunjun
