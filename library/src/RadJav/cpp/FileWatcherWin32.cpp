#include "cpp/fileWatcherWin32.h"


#if FILEWATCHER_PLATFORM == FILEWATCHER_PLATFORM_WIN32

#define _WIN32_WINNT 0x0550
#include <windows.h>
#include <algorithm>

#if defined(_MSC_VER)
	#pragma comment(lib, "comctl32.lib")
	#pragma comment(lib, "user32.lib")
	#pragma comment(lib, "ole32.lib")

	// disable secure warnings
	#pragma warning (disable: 4996)
#endif


namespace FW
{
	struct watchData
	{
		OVERLAPPED overlapped;
		HANDLE handle;
		BYTE buffer[32 * 1024];
		LPARAM lparam;
		unsigned long notificationFilter;
		bool stop;
		fileWatcherImpl* impl;
		updateListener* listener;
		char* path;
		watchID id;
		bool recursive;
	};


	#pragma region Internal Functions

	bool updateWatch(watchData* ptr_, bool clear_ = false);

	void CALLBACK watchCallback(unsigned long error_, unsigned long bytesTransferred_, LPOVERLAPPED overlapped_)
	{
		char file[MAX_PATH];

		PFILE_NOTIFY_INFORMATION notify;
		watchData* ptr = (watchData*)overlapped_;
		size_t offset = 0;


		if (bytesTransferred_ == 0)
			return;

		if (error_ == ERROR_SUCCESS)
		{
			do
			{
				notify = (PFILE_NOTIFY_INFORMATION)&ptr->buffer[offset];
				offset += notify->NextEntryOffset;

				int count = WideCharToMultiByte(CP_ACP, 
												0, 
												notify->FileName,
												notify->FileNameLength / sizeof(WCHAR),
												file,
												MAX_PATH - 1, 
												NULL, 
												NULL);
				file[count] = TEXT('\0');

				ptr->impl->handleAction(ptr, file, notify->Action);

			} while (notify->NextEntryOffset != 0);
		}

		if (!ptr->stop) updateWatch(ptr);
	}

	bool updateWatch(watchData* ptr_, bool clear_)
	{
		return ReadDirectoryChangesW(ptr_->handle,
									 ptr_->buffer,
									 sizeof(ptr_->buffer),
									 ptr_->recursive,
									 ptr_->notificationFilter,
									 NULL, 
									 &ptr_->overlapped,
									 clear_ ? 0 : watchCallback) != 0;
	}

	void destroyWatch(watchData* ptr_)
	{
		if (ptr_)
		{
			ptr_->stop = TRUE;

			CancelIo(ptr_->handle);
			updateWatch(ptr_, true);

			if (!HasOverlappedIoCompleted(&ptr_->overlapped))
				SleepEx(5, TRUE);

			CloseHandle(ptr_->overlapped.hEvent);
			CloseHandle(ptr_->handle);
			
			delete ptr_->path;
			
			HeapFree(GetProcessHeap(), 0, ptr_);
		}
	}

	watchData* createWatch(LPCTSTR path_, bool recursive_, unsigned long filter)
	{
		watchData* ptr;
		size_t ptrSize = sizeof(*ptr);
		ptr = static_cast<watchData*>(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ptrSize));


		ptr->handle = CreateFile(path_,
								 FILE_LIST_DIRECTORY,
								 FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
								 NULL, 
								 OPEN_EXISTING, 
								 FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, 
								 NULL);

		if (ptr->handle != INVALID_HANDLE_VALUE)
		{
			ptr->overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			ptr->notificationFilter = filter;
			ptr->recursive = recursive_;

			if (updateWatch(ptr))
				return ptr;
			else
			{
				CloseHandle(ptr->overlapped.hEvent);
				CloseHandle(ptr->handle);
			}
		}

		HeapFree(GetProcessHeap(), 0, ptr);
		
		
		return NULL;
	}

	#pragma endregion


	fileWatcherWin32::fileWatcherWin32() : m_lastWatchID(0)
	{
	}

	fileWatcherWin32::~fileWatcherWin32()
	{
		watchMap::iterator iter = m_watches.begin();
		watchMap::iterator end = m_watches.end();
		for (; iter != end; ++iter)
			destroyWatch(iter->second);

		m_watches.clear();
	}

	void fileWatcherWin32::add(const std::string& path_, updateListener* listener_, bool recursive_)
	{
		//check for duplicate adds
		for (const auto& f : m_watches)
			if (path_ == f.second->path)
				return;
	
		watchID id = ++m_lastWatchID;

		std::wstring stemp = std::wstring(path_.begin(), path_.end());
		watchData* watch = createWatch(stemp.c_str(),
									   recursive_,
									   FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_FILE_NAME);

		if (!watch) return;

		watch->id = id;
		watch->impl = this;
		watch->listener = listener_;
		watch->path = new char[path_.length()+1];
		strcpy(watch->path, path_.c_str());

		m_watches.insert(std::make_pair(id, watch));
	}

	void fileWatcherWin32::remove(const std::string& path_)
	{
		watchMap::iterator iter = m_watches.begin();
		watchMap::iterator end = m_watches.end();
		for (; iter != end; ++iter)
		{
			if (path_ == iter->second->path)
			{
				remove(iter->first);
				return;
			}
		}
	}

	void fileWatcherWin32::remove(watchID id_)
	{
		watchMap::iterator iter = m_watches.find(id_);

		if (iter == m_watches.end())
			return;

		watchData* watch = iter->second;
		m_watches.erase(iter);

		destroyWatch(watch);
	}

	void fileWatcherWin32::update()
	{
		MsgWaitForMultipleObjectsEx(0, NULL, 0, QS_ALLINPUT, MWMO_ALERTABLE);
	}

	void fileWatcherWin32::handleAction(watchData* watch_, const std::string& filename_, unsigned long action_)
	{
		actions action;

		switch (action_)
		{
		case FILE_ACTION_RENAMED_NEW_NAME:
		case FILE_ACTION_ADDED:
			action = actions::added;
			break;
		case FILE_ACTION_RENAMED_OLD_NAME:
		case FILE_ACTION_REMOVED:
			action = actions::deleted;
			break;
		case FILE_ACTION_MODIFIED:
			action = actions::modified;
			break;
		};

		watch_->listener->handleFileAction(watch_->id, watch_->path, filename_, action);
	}
};

#endif//_WIN32