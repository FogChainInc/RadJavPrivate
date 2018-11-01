#include "cpp/fileWatcher.h"
#include "cpp/fileWatcherImpl.h""
#include "boost/filesystem.hpp"

#if FILEWATCHER_PLATFORM == FILEWATCHER_PLATFORM_WIN32
	#include "cpp/fileWatcherWin32.h""
	#define FILEWATCHER_IMPL fileWatcherWin32
#elif FILEWATCHER_PLATFORM == FILEWATCHER_PLATFORM_LINUX
	#include <cpp/fileWatcherLinux.h>
	#define FILEWATCHER_IMPL fileWatcherLinux
#elif FILEWATCHER_PLATFORM == FILEWATCHER_PLATFORM_KQUEUE
	#include <cpp/fileWatcherOSX.h>
	#define FILEWATCHER_IMPL fileWatcherOSX
#endif


std::vector<FW::fileWatch> FW::updateListener::m_fileWatches;


namespace FW
{
	updateListener::updateListener(const std::string file_, const std::string path_, callback_functor callback_)
	{
		//check for duplicates
		for (const auto& f : updateListener::m_fileWatches)
			if (path_ == f.path && (f.path == f.file || file_ == f.file))
				return;

		updateListener::m_fileWatches.push_back(fileWatch{ path_, file_, callback_ });
	}

	void updateListener::handleFileAction(FW::watchID id_, const std::string& path_, const std::string& file_, FW::actions action_)
	{
		//match the path & file or just the path for all files in that folder
		for (const auto& f : updateListener::m_fileWatches)
			if (path_ == f.path && (f.path == f.file || file_ == f.file))
			{
				f.callback(path_ + "\\" + file_, (int)action_);
			}
	}


	fileWatcher::fileWatcher()
	{
		m_impl = new FILEWATCHER_IMPL();
	}

	fileWatcher::~fileWatcher()
	{
		delete m_impl;
		m_impl = nullptr;
	}

	void fileWatcher::startUpdating()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (!m_updating)
		{
			boost::asio::post(FW::m_watchQueue, [=]()
			{
				while (true)
					update();
			});

			m_updating = true;
		}
	}

	void fileWatcher::addFile(const std::string& file_, callback_functor callback_)
	{
		boost::filesystem::path p(file_);
		boost::filesystem::path path = p.parent_path();

		m_impl->add(path.string(), new updateListener(file_, path.string(), callback_), false);

		startUpdating();
	}

	void fileWatcher::addFolder(const std::string& path_, bool recursive_, callback_functor callback_)
	{
		m_impl->add(path_, new updateListener(path_, path_, callback_), recursive_);

		startUpdating();
	}

	void fileWatcher::remove(const std::string& path_)
	{
		m_impl->remove(path_);
	}

	void fileWatcher::update()
	{
		m_impl->update();
	}
};