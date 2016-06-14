
#include <Dunjun/FileSystem.hpp>

#include <SDL/SDL_filesystem.h>

namespace Dunjun
{
	namespace FileSystem
	{
		std::string getBasePath()
		{
			return SDL_GetBasePath();
		}

		std::string getPreferencesPath(const std::string organization, const std::string application)
		{
			return SDL_GetPrefPath(organization.c_str(), application.c_str());
		}

		std::string getFileDirectory(const std::string& filepath)
		{
			size_t found = filepath.find_last_of("/\\");
			return filepath.substr(0, found);
		}

	} // end FileSystem
} // end Dunjun
