
#include <Dunjun/System/Logger.hpp>

#ifdef DUNJUN_SYSTEM_WINDOWS
#include <Windows.h>
#endif


namespace Dunjun
{

	Logger g_loggerDebug;
	Logger g_loggerError;
	Logger g_loggerInfo;
	Logger g_loggerEvent;
	Logger g_loggerWindow;
	Logger g_loggerInput;
	Logger g_loggerConsole;
	Logger g_loggerModel;
	Logger g_loggerRender;
	Logger g_loggerGame;

	INTERNAL void logSetConsoleColor(const u32& logFlags)
	{
#if defined(DUNJUN_SYSTEM_WINDOWS)
		u8 textColor = 7;
		u8 backgroundColor = 0;
		//	base color value:
		//		0~6 are dark colors in order
		//			black, blue, green, cyan, red, magenta, yellow
		//	7~8 are light grey and dark grey
		//	9~15 are full brightness colors in order
		//			blue, green, cyan, red, magenta, yellow, white
		//
		// color format is (u8)(textColor + backgroundColor * 16)

		if (logFlags & LogFlag::LogFlag_ColorBackground_Grey)
			backgroundColor = 8;

		// text color
		{
			if (logFlags & LogFlag::LogFlag_ColorText_White)
				textColor = 15;
			else if (logFlags & LogFlag::LogFlag_ColorText_Green)
				textColor = 10;
			else if (logFlags & LogFlag::LogFlag_ColorText_Red)
				textColor = 12;
			else if (logFlags & LogFlag::LogFlag_ColorText_Yellow)
				textColor = 14;
			else if (logFlags & LogFlag::LogFlag_ColorText_Cyan)
				textColor = 11;
			else if (logFlags & LogFlag::LogFlag_ColorText_Magenta)
				textColor = 13;
			else if (logFlags & LogFlag::LogFlag_ColorText_Blue)
				textColor = 9;
			else if (logFlags & LogFlag::LogFlag_ColorText_Black)
				textColor = 0;
		}

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, textColor + backgroundColor * 16);
#else
		// TODO: Terminal color not implemented for this operating system
#endif // end set up print environment
	}
	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					PRINT FUNCTION
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	void logPrint(const Logger& logger, const char* fmt, ...)
	{
		if(logger.flags & LogFlag::LogFlag_Disable)
			return;

		LOCAL_PERSIST char s_buffer[1024] = {};

		// add new entry line
		if (logger.flags & LogFlag::LogFlag_Text_NewLine)
			strcpy(s_buffer, "\n");
		else
			strcpy(s_buffer, " ");

		if(logger.prefix != nullptr)
		{
			strncat(s_buffer, logger.prefix, strlen(logger.prefix));
			strncat(s_buffer, " " , 1);
		}
		else
		{
			strcpy(s_buffer, "");
		}
		
		// get extra data to be printed
		{
			time_t rawTime = {};
			struct tm* timeInfo = nullptr;

			if(logger.flags & LogFlag::LogFlag_Date | logger.flags & LogFlag::LogFlag_Time)
			{
				time(&rawTime);
				timeInfo = localtime(&rawTime);
			}

			// collect data in buffer
			if(logger.flags & LogFlag::LogFlag_Date)
			{
				LOCAL_PERSIST char s_dateBuffer[13] = {};
				_snprintf(s_dateBuffer, 13, "%04d-%02d-%02d ", 
						  1900 + timeInfo->tm_year, timeInfo->tm_mon + 1, timeInfo->tm_mday);

				strncat(s_buffer, s_dateBuffer, 13);
			}

			if (logger.flags & LogFlag::LogFlag_Time)
			{
				LOCAL_PERSIST char s_timeBuffer[13] = {};
				_snprintf(s_timeBuffer, 13, "%02d:%02d:%02d ",
						  timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);

				strncat(s_buffer, s_timeBuffer, 13);
			}
		} // end get extra data to be printed

		strncat(s_buffer, ">> ", 3);
		strncat(s_buffer, fmt, strlen(fmt));

		// set up print environment
		logSetConsoleColor(logger.flags);

		// Start printing
		va_list v;
		va_start(v, fmt);
		{
			if(logger.flags & LogFlag::LogFlag_SaveToFile)
				vfprintf(logger.file, s_buffer, v);

			if(logger.flags & LogFlag::LogFlag_PrintToTerminal)
				vprintf(s_buffer, v);
		}
		va_end(v);
	}

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					SECTION FUNCTION
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	void logSection(const Logger& logger, const char* sectionName)
	{
		if (logger.flags & LogFlag::LogFlag_Disable)
			return;

		LOCAL_PERSIST char s_buffer[1024] = {};

		// add new entry line
		if (logger.flags & LogFlag::LogFlag_Text_NewLine)
			strcpy(s_buffer, "\n");
		else
			strcpy(s_buffer, " ");

		const char* sep = "========================================================";
		uSize_t lname = strlen(sectionName);
		uSize_t lsep = strlen(sep);

		// add first seperator
		strncat(s_buffer, sep, lsep);

		// add new line
		if (logger.flags & LogFlag::LogFlag_Text_NewLine)
			strncat(s_buffer, "\n", 1);

		// add logger prefix
		if (logger.prefix != nullptr)
		{
			strncat(s_buffer, logger.prefix, strlen(logger.prefix));
			strncat(s_buffer, " ", 1);
		}
		else
		{
			strcpy(s_buffer, "");
		}

		// get extra data to be printed
		{
			time_t rawTime = {};
			struct tm* timeInfo = nullptr;

			// collect data in buffer
			{
				time(&rawTime);
				timeInfo = localtime(&rawTime);

				// date
				LOCAL_PERSIST char s_dateBuffer[13] = {};
				_snprintf(s_dateBuffer, 13, "%04d-%02d-%02d ",
					1900 + timeInfo->tm_year, timeInfo->tm_mon + 1, timeInfo->tm_mday);

				strncat(s_buffer, s_dateBuffer, 13);

				// time
				LOCAL_PERSIST char s_timeBuffer[13] = {};
				_snprintf(s_timeBuffer, 13, "%02d:%02d:%02d ",
					timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
				
				strncat(s_buffer, s_timeBuffer, 13);
			}
		} // end get extra data to be printed

		 // add name
		strncat(s_buffer, ">> ", 3);
		strncat(s_buffer, sectionName, lname);

		// add new line
		if (logger.flags & LogFlag::LogFlag_Text_NewLine)
			strncat(s_buffer, "\n", 1);

		// add second seperator
		strncat(s_buffer, sep, lsep);

		// set up print environment
		logSetConsoleColor(logger.flags);

		// print seperator
		if (logger.flags & LogFlag::LogFlag_SaveToFile)
			fprintf(logger.file, s_buffer);
		if (logger.flags & LogFlag::LogFlag_PrintToTerminal)
			printf(s_buffer);
	}

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					HELPER FUNCTIONS
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	void setLogger(Logger& logger, FILE* file, const char* prefix, u32 flags)
	{
		logger.file = file;
		logger.prefix = prefix;
		logger.flags = flags;
	}

	void setLoggerColor(Logger& logger, const LogFlag& textColor, const LogFlag& backgroundColor)
	{
		assert(textColor >= LogFlag::LogFlag_ColorText_Blue && textColor <= LogFlag::LogFlag_ColorText_Black && 
			"setLoggerClor() textColor must be LogFlag_ColorText");
		assert(backgroundColor >= LogFlag::LogFlag_ColorBackground_Grey && backgroundColor <= LogFlag::LogFlag_ColorBackground_Black && 
			"setLoggerColor() backgroundColor must be LogFlag_BackgroundColor");
		assert((textColor & (textColor - 1)) == 0 && "setLoggerColor() textColor must be power of 2");
		assert((backgroundColor & (backgroundColor - 1)) == 0 && "setLoggerColor() backgroundColor must be a power of 2");

		// if logger is disabled do nothing
		if(logger.flags & LogFlag::LogFlag_Disable)
			return;

		// remove any existing color flags
		{
			// background color
			if (logger.flags & LogFlag::LogFlag_ColorBackground_Grey)
				logger.flags -= LogFlag::LogFlag_ColorBackground_Grey;
			if (logger.flags & LogFlag::LogFlag_ColorBackground_Black)
				logger.flags -= LogFlag::LogFlag_ColorBackground_Black;

			// text color
			if (logger.flags & LogFlag::LogFlag_ColorText_Blue)
				logger.flags -= LogFlag::LogFlag_ColorText_Blue;
			if (logger.flags & LogFlag::LogFlag_ColorText_Green)
				logger.flags -= LogFlag::LogFlag_ColorText_Green;
			if (logger.flags & LogFlag::LogFlag_ColorText_Cyan)
				logger.flags -= LogFlag::LogFlag_ColorText_Cyan;
			if (logger.flags & LogFlag::LogFlag_ColorText_Red)
				logger.flags -= LogFlag::LogFlag_ColorText_Red;
			if (logger.flags & LogFlag::LogFlag_ColorText_Magenta)
				logger.flags -= LogFlag::LogFlag_ColorText_Magenta;
			if (logger.flags & LogFlag::LogFlag_ColorText_Yellow)
				logger.flags -= LogFlag::LogFlag_ColorText_Yellow;
			if (logger.flags & LogFlag::LogFlag_ColorText_White)
				logger.flags -= LogFlag::LogFlag_ColorText_White;
			if (logger.flags & LogFlag::LogFlag_ColorText_Black)
				logger.flags -= LogFlag::LogFlag_ColorText_Black;
		}

		logger.flags |= backgroundColor;
		logger.flags |= textColor;
	}

} // end Dunjun
