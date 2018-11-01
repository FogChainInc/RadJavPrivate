#ifndef _FILEWATCHER_H_
#define _FILEWATCHER_H_
#pragma once


#include <string>
#include <stdexcept>
#include <functional>
#include <iostream>
#include <vector>
#include <mutex>

#ifdef USE_BOOST
	#include <boost/asio.hpp>
#endif


namespace FW
{
	#ifdef USE_BOOST
		static boost::asio::thread_pool m_watchQueue(4);
	#endif

	typedef unsigned long watchID;

	using callback_functor = std::function<void(const std::string& file_, const int action_)>;

	enum class actions : int { added = 1, deleted, modified };

	class fileWatcherImpl;

	struct fileWatch
	{
		std::string path;
		std::string file;
		callback_functor callback;
	};

	class updateListener
	{
	public:
		updateListener(const std::string file_, const std::string path_, callback_functor callback_);
		~updateListener() = default;

		void handleFileAction(FW::watchID id_, const std::string& path_, const std::string& filename_, FW::actions action_);
	
	private:
		static std::vector<fileWatch> m_fileWatches;
	};


	class fileWatcher
	{
	public:
		fileWatcher();
		virtual ~fileWatcher();

		void addFile(const std::string& file_, callback_functor callback_);
		void addFolder(const std::string& path_, bool recursive_, callback_functor callback_);
		void remove(const std::string& path_);

		void update();

	private:
		void startUpdating();

		fileWatcherImpl* m_impl;
		bool m_updating = false;

		std::mutex m_mutex;
	};
};

#endif //_FILEWATCHER_H_