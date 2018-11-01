#include <cpp/fileWatcherLinux.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>


#if FILEWATCHER_PLATFORM == FILEWATCHER_PLATFORM_LINUX
#include <unistd.h>
#include <sys/inotify.h>


#define BUFFER_SIZE (FILENAME_MAX + (sizeof(struct inotify_event)) * 1024)


namespace FW
{
	struct watchData
	{
		WatchID watchID;
		String path;
		updateListener* listener;		
	};

	fileWatcherLinux::fileWatcherLinux()
	{
		m_FD = inotify_init();
		
		m_timeout.tv_sec = 0;
		m_timeout.tv_usec = 0;
	   		
		FD_ZERO(&m_descriptorSet);
	}

	fileWatcherLinux::~fileWatcherLinux()
	{
		watchMap::iterator iter = m_watches.begin();
		watchMap::iterator end = m_watches.end();
		for(; iter != end; ++iter)
			delete iter->second;
		m_watches.clear();
	}

	WatchID fileWatcherLinux::add(const String& path_, updateListener* listener_, bool recursive_)
	{
		int wd = inotify_add_watch(mFD,
								   directory.c_str(), 
								   IN_CLOSE_WRITE | IN_MOVED_TO | IN_CREATE | IN_MOVED_FROM | IN_DELETE);
		if (wd < 0)
			return;
		
		watchData* ptr = new watchData();
		ptr->listener = watcher;
		ptr->watchID = wd;
		ptr->path = directory;
		
		m_watches.insert(std::make_pair(wd, ptr));
	
		return wd;
	}

	void fileWatcherLinux::remove(const String& directory)
	{
		watchMap::iterator iter = m_watches.begin();
		watchMap::iterator end = m_watches.end();
		for (; iter != end; ++iter)
		{
			if (directory == iter->second->mDirName)
			{
				remove(iter->first);
				return;
			}
		}
	}

	void fileWatcherLinux::removeWatch(watchID watchid)
	{
		watchMap::iterator iter = m_watches.find(watchid);

		if (iter == m_watches.end())
			return;

		watchData* watch = iter->second;
		m_watches.erase(iter);
	
		inotify_rm_watch(m_FD, watchid);
		
		delete watch;
		watch = nullptr;
	}

	void fileWatcherLinux::update()
	{
		FD_SET(m_FD, &m_descriptorSet);

		int ret = select(mFD + 1, &m_descriptorSet, NULL, NULL, &m_timeout);
		if (ret < 0)
		{
			perror("select");
		}
		else if (FD_ISSET(mFD, &m_descriptorSet))
		{
			ssize_t len, i = 0;
			char action[FILENAME_MAX + 81] = {0};
			char buffer[BUFFER_SIZE] = {0};

			len = read(mFD, buff, BUFFER_SIZE);
		   
			while (i < len)
			{
				struct inotify_event *evt = (struct inotify_event *)&buffer[i];

				watchData* watch = m_watches[evt->wd];
				handleAction(watch, evt->name, evt->mask);
				i += sizeof(struct inotify_event) + evt->len;
			}
		}
	}

	void fileWatcherLinux::handleAction(watchData* watch, const String& filename, unsigned long action)
	{
		if (!watch->listener)
			return;

		if (IN_CLOSE_WRITE & action)
		{
			watch->listener->handleFileAction(watch->mWatchID,
											  watch->mDirName, 
											  filename,
											  Actions::Modified);
		}

		if (IN_MOVED_TO & action || IN_CREATE & action)
		{
			watch->listener->handleFileAction(watch->mWatchID,
											  watch->mDirName, 
											  filename,	
											  Actions::Add);
		}

		if (IN_MOVED_FROM & action || IN_DELETE & action)
		{
			watch->listener->handleFileAction(watch->mWatchID,
											  watch->mDirName, 
											  filename,
											  Actions::Delete);
		}
	}
};

#endif//FILEWATCHER_PLATFORM_LINUX
