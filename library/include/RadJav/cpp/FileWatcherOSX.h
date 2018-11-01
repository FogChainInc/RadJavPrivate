#ifndef _FW_FILEWATCHEROSX_H_
#define _FW_FILEWATCHEROSX_H_
#pragma once


#include "FileWatcherImpl.h"
#include <map>
#include <sys/types.h>


#if FILEWATCHER_PLATFORM == FILEWATCHER_PLATFORM_KQUEUE
	namespace FW
	{
		class fileWatcherOSX : public fileWatcherImpl
		{
		public:
			typedef std::map<watchID, watchData*> watchMap;

			fileWatcherOSX();
			virtual ~fileWatcherOSX();

			void add(const std::string& path_, updateListener* listener_, bool recursive_);
			void remove(const std::string& path_);
			void remove(watchID id_);

			void update();

			void handleAction(watchData* watch_, const std::string& filename_, unsigned long action_);

		private:
			watchMap m_watches;
			watchID m_lastWatchID;
			int m_descriptor;
			struct timespec m_timeout;
		};
	};
#endif

#endif //_FW_FILEWATCHEROSX_H_
