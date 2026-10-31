#pragma once
#include "Core/PlatformDetection.h"
#ifdef SIX_WINDOWS
	#ifdef SIX_EXPORT
		#define SIXAPI __declspec(dllexport)
	#elif defined(SIX_STATIC)
		#define SIXAPI
	#else
		#define SIXAPI __declspec(dllimport)
	#endif
#endif