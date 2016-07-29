#ifndef DUNJUN_SYSTEM_CONFIG_HPP
#define DUNJUN_SYSTEM_CONFIG_HPP

///////////////////////
//		SYSTEM OS
///////////////////////

#if defined(_WIN32) || defined(_WIN64)
// windows
	#define DUNJUN_SYSTEM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif

#elif defined(__APPLE__) && defined(__MACH__)
	#define DUNJUN_SYSTEM_OSX

#elif defined(__unix__)
	#define DUNJUN_SYSTEM_UNIX
	#if defined(__linux__)
		#define DUNJUN_SYSTEM_LINUX

	#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
		#define DUNJUN_SYSTEM_FREEBSD

	#else
	//unsupported UNIX system
	#error This Unix operating system is not supported by Dunjun
	#endif

#else
	//unsupported operating system
	#error This operating system is not supported by Dunjun

#endif

///////////////////////
// ENVIRONMENT BIT SIZE
///////////////////////

#if defined(_WIN32) || defined(_WIN64)
	#if defined(_WIN64)
		#define DUNJUN_64_BIT
	#else
		#define DUNJUN_32_BIT
	#endif
#endif

#if defined(__GNUC__)
	#if defined(__x86_64__) || defined(__ppc54__)
		#define DUNJUN_64_BIT
	#else
		#defined DUNJUN_32_BIT
	#endif
#endif

///////////////////////
//	   COMPILIER
///////////////////////

#if defined(_MSC_VER)
	// Microsoft Visual Studio
	#pragma comment(lib, "winmm.lib") // used in Time.cpp
	#define DUNJUN_COMPILER_MSVC
#elif defined(__clang__)
	// Clang
	#defined DUNJUN_COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
	// GNU GCC/G++ Compiler
	#defined DUNJUN_COMPILER_GNU_GCC
#endif

///////////////////////
//       DEBUG
///////////////////////

#define DUNJUN_DEBUG_MODE

#ifdef DUNJUN_DEBUG_MODE
	#define STOP {char* createAccessViolation = 0; *createAccessViolation = 0;}
#else
	#define STOP // create access violation in debug mode only
#endif


#endif
