#pragma once

#ifdef EV_PLATFORM_WINDOWS
	#ifdef EV_BUILD_DLL
		#define ENGINEV_API __declspec(dllexport)
	#else
		#define	ENGINEV_API __declspec(dllimport)
	#endif	
#else
	#error EngineV only supports Windows
#endif