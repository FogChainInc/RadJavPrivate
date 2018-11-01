#ifndef _FW_FILEWATCHERIMPL_H_
#define _FW_FILEWATCHERIMPL_H_
#pragma once

#include "fileWatcher.h"


#define FILEWATCHER_PLATFORM_WIN32 1
#define FILEWATCHER_PLATFORM_LINUX 2
#define FILEWATCHER_PLATFORM_KQUEUE 3


#if defined(_WIN32)
	#define FILEWATCHER_PLATFORM FILEWATCHER_PLATFORM_WIN32
#elif defined(__APPLE_CC__) || defined(BSD)
	#define FILEWATCHER_PLATFORM FILEWATCHER_PLATFORM_KQUEUE
#elif defined(__linux__)
	#define FILEWATCHER_PLATFORM FILEWATCHER_PLATFORM_LINUX
#endif


namespace FW
{
	struct watchData;

	class fileWatcherImpl
	{
	public:
		fileWatcherImpl() {}
		virtual ~fileWatcherImpl() {}

		virtual void add(const std::string& path_, updateListener* listener_, bool recursive_) = 0;
		virtual void remove(const std::string& path_) = 0;

		virtual void update() = 0;

		virtual void handleAction(watchData* watch_, const std::string& filename_, unsigned long action_) = 0;
	};
};

#endif//_FW_FILEWATCHERIMPL_H_