#ifndef DUNJUN_CONFIG_HPP
#define DUNJUN_CONFIG_HPP

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

#endif
