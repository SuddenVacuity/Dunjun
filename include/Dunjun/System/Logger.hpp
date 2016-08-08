#ifndef DUNJUN_SYSTEM_LOGGER_HPP
#define DUNJUN_SYSTEM_LOGGER_HPP

#include <Dunjun/System/Types.hpp>

namespace Dunjun
{
	enum LogFlag : u32
	{
		LogFlag_Disable = 1073741824,

		LogFlag_PrintToTerminal = 1,
		LogFlag_SaveToFile = 2,

		LogFlag_Date = 4,
		LogFlag_Time = 8,

		LogFlag_ColorBackground_Grey = 16,
		LogFlag_ColorBackground_Black = 32,

		LogFlag_ColorText_Blue    = 64,
		LogFlag_ColorText_Green   = 128,
		LogFlag_ColorText_Cyan    = 256,
		LogFlag_ColorText_Red     = 512,
		LogFlag_ColorText_Magenta = 1024,
		LogFlag_ColorText_Yellow  = 2048,
		LogFlag_ColorText_White   = 4096,
		LogFlag_ColorText_Black   = 8192,
																						
		LogFlag_PresetDefault = LogFlag_Date | LogFlag_Time | LogFlag_PrintToTerminal,	
	};																					
																						
																						
	struct Logger																		
	{																					
		FILE* file;																		
		u32 flags;																		
		const char* prefix;																
	};																					
																						  
	void logPrint(const Logger& log, const char* fmt, ...);
	void setLogger(Logger& logger, FILE* file, const char* prefix, u32 flags);

	void setLoggerColor(Logger& logger, const LogFlag& textColor, const LogFlag& backgroundColor);

} // end Dunjun

#endif
