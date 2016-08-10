
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

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					GET SYSTEM OS
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifdef DUNJUN_SYSTEM_WINDOWS
#include <Windows.h>
#endif

	const char* getSystemOS()
	{
#ifdef DUNJUN_SYSTEM_WINDOWS
		OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0,{ 0 }, 0, 0 };
		DWORDLONG const dwlConditionalMask = VerSetConditionMask(VerSetConditionMask(0, VER_MAJORVERSION, VER_EQUAL), VER_MINORVERSION, VER_EQUAL);

		bool match = 0;

		osvi.dwMajorVersion = HIBYTE(OSVersion::WINDOWS_10);
		osvi.dwMinorVersion = LOBYTE(OSVersion::WINDOWS_10);
		match = VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionalMask);
		if (match)
			return "Windows 10 NT 10.0";

		osvi.dwMajorVersion = HIBYTE(OSVersion::WINDOWS_8_1);
		osvi.dwMinorVersion = LOBYTE(OSVersion::WINDOWS_8_1);
		match = VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionalMask);
		if (match)
			return "Windows 8.1 NT 6.3";

		osvi.dwMajorVersion = HIBYTE(OSVersion::WINDOWS_8);
		osvi.dwMinorVersion = LOBYTE(OSVersion::WINDOWS_8);
		match = VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionalMask);
		if (match)
			return "Windows 8 NT 6.2";

		osvi.dwMajorVersion = HIBYTE(OSVersion::WINDOWS_7);
		osvi.dwMinorVersion = LOBYTE(OSVersion::WINDOWS_7);
		match = VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionalMask);
		if (match)
			return "Windows 7 NT 6.1";

		osvi.dwMajorVersion = HIBYTE(OSVersion::WINDOWS_VISTA);
		osvi.dwMinorVersion = LOBYTE(OSVersion::WINDOWS_VISTA);
		match = VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionalMask);
		if (match)
			return "Windows Vista NT 6.0";

		osvi.dwMajorVersion = HIBYTE(OSVersion::WINDOWS_XP64);
		osvi.dwMinorVersion = LOBYTE(OSVersion::WINDOWS_XP64);
		match = VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionalMask);
		if (match)
			return "Windows XP Professional x64 NT 5.2";

		osvi.dwMajorVersion = HIBYTE(OSVersion::WINDOWS_XP);
		osvi.dwMinorVersion = LOBYTE(OSVersion::WINDOWS_XP);
		match = VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionalMask);
		if (match)
			return "Windows XP NT 5.1";

		osvi.dwMajorVersion = HIBYTE(OSVersion::WINDOWS_ME);
		osvi.dwMinorVersion = LOBYTE(OSVersion::WINDOWS_ME);
		match = VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionalMask);
		if (match)
			return "Windows ME 4.90";

		osvi.dwMajorVersion = HIBYTE(OSVersion::WINDOWS_2000);
		osvi.dwMinorVersion = LOBYTE(OSVersion::WINDOWS_2000);
		match = VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionalMask);
		if (match)
			return "Windows 2000 NT 5.0";

		// no match was found
		return "Windows - Unknown version";

#else
		// TODO: getSystemOS() check for non-windows operating systems
		return "getSystemOS() not supported on this OS";
#endif
	}



} // end Dunjun
