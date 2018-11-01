#ifndef _FW_FILEWATCHERWIN32_H_
#define _FW_FILEWATCHERWIN32_H_
#pragma once


#include "fileWatcherImpl.h"
#include <map>


#if FILEWATCHER_PLATFORM == FILEWATCHER_PLATFORM_WIN32
	namespace FW
	{
		class fileWatcherWin32 : public fileWatcherImpl
		{
		public:
			typedef std::map<watchID, watchData*> watchMap;

			fileWatcherWin32();
			virtual ~fileWatcherWin32();

			void add(const std::string& path_, updateListener* listener_, bool recursive_);
			void remove(const std::string& path_);
			void remove(watchID id_);

			void update();

			void handleAction(watchData* watch_, const std::string& filename_, unsigned long action_);

		private:
			watchMap m_watches;
			watchID m_lastWatchID;
		};
	};
#endif

#endif //FW_FILEWATCHERWIN32_H_