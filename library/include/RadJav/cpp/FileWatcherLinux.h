#ifndef _FW_FILEWATCHERLINUX_H_
#define _FW_FILEWATCHERLINUX_H_
#pragma once


#include "FileWatcherImpl.h"
#include <map>
#include <sys/types.h>


#if FILEWATCHER_PLATFORM == FILEWATCHER_PLATFORM_LINUX
	namespace FW
	{
		class fileWatcherLinux : public fileWatcherImpl
		{
		public:
			typedef std::map<watchID, watchData*> watchMap;

			fileWatcherLinux();
			virtual ~fileWatcherLinux();

			void add(const std::string& path_, updateListener* listener_, bool recursive_);
			void remove(const std::string& path_);
			void remove(watchID id_);

			void update();

			void handleAction(watchData* watch_, const std::string& filename_, unsigned long action_);

		private:
			watchMap m_watches;
			watchID m_lastWatchID;
			int m_FD;
			struct timeval m_timeout;
			fd_set m_descriptorSet;
		};
	};
#endif

#endif //_FW_FILEWATCHERLINUX_H_
