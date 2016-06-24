
#include <Dunjun/Common.hpp>

#include <SDL\SDL.h>


namespace Dunjun
{
	//std::string getFileDirectory(const std::string& filepath)
	//{
	//	size_t found = filepath.find_last_of("/\\");
	//	return filepath.substr(0, found);
	//}

	bool showSimpleMessageBox(MessageBoxType type,
		const std::string& title,
		const std::string& message)
	{
		u32 flag = 0;

		switch(type)
		{
		default: break;
		case MessageBoxType::Error		 : flag = SDL_MESSAGEBOX_ERROR;		  break;
		case MessageBoxType::Warning	 : flag = SDL_MESSAGEBOX_WARNING;	  break;
		case MessageBoxType::Information : flag = SDL_MESSAGEBOX_INFORMATION; break;
		}

		return SDL_ShowSimpleMessageBox(flag, title.c_str(), message.c_str(), nullptr) == 0;
	}

	namespace BaseDirectories
	{
		const std::string Shaders = "data/shaders/";
		const std::string Texture = "data/textures/";
	} // end BaseDirectories
} // end Dunjun
