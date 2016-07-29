
#include <Dunjun/Common.hpp>

#include <SDL\SDL.h>


namespace Dunjun
{
	//std::string getFileDirectory(const std::string& filepath)
	//{
	//	size_t found = filepath.find_last_of("/\\");
	//	return filepath.substr(0, found);
	//}

	bool showSimpleMessageBox(const String& message,
							  const String& title,
							  MessageBoxType type)
	{
		u32 flag = 0;

		switch(type)
		{
		default: break;
		case MessageBoxType::Error		 : flag = SDL_MESSAGEBOX_ERROR;		  break;
		case MessageBoxType::Warning	 : flag = SDL_MESSAGEBOX_WARNING;	  break;
		case MessageBoxType::Information : flag = SDL_MESSAGEBOX_INFORMATION; break;
		}

		bool result = SDL_ShowSimpleMessageBox(flag, cString(title), cString(message), nullptr) == 0;

		if(type == MessageBoxType::Error)
		{
			STOP
			std::exit(EXIT_FAILURE);
		}

		return result;
	}
} // end Dunjun
