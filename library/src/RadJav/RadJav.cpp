/*
	MIT-LICENSE
	Copyright (c) 2017 Higher Edge Software, LLC

	Permission is hereby granted, free of charge, to any person obtaining a copy of this software
	and associated documentation files (the "Software"), to deal in the Software without restriction,
	including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
	and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial
	portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
	LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifdef USE_BLOCKCHAIN_V1
	#include "xrjv1d.h"
	#include "xrjv1-cli.h"
	#include "noui.h"
	#include "util.h"
#endif

#include "RadJav.h"

#ifdef GUI_USE_WXWIDGETS
	IMPLEMENT_APP_NO_MAIN(RadJAV::wxWidgetsRadJav)
#else
	#if WIN32
		#include <Windows.h>
		#include <io.h>
	#endif

	#if defined USE_IOS || defined USE_ANDROID
		#include "cpp/RadJavCPPMUIAlert.h"
	#endif

	#include <stdlib.h>
	#include <fcntl.h>
#endif

#ifdef USE_V8
	#include "v8/RadJavV8JavascriptEngine.h"
#endif

#ifdef USE_JAVASCRIPTCORE
    #include "jscore/RadJavJSCJavascriptEngine.h"
#endif

#ifdef HTTP_USE_CIVETWEB
	#include <civetweb.h>
#endif

#ifdef HTTP_USE_CURL
	#include <curl/curl.h>
#endif

#include "cpp/RadJavCPPIO.h"
#include "cpp/RadJavCPPOS.h"

#include <new>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#ifdef __ANDROID__
#include <android/log.h>
#define  LOG_TAG    "RADJAV"

#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#else
#define  LOGE()
#define  LOGW()
#define  LOGD()
#define  LOGI()
#endif

#ifdef RADJAV_DEBUG
#ifdef LOG_MEMORY_LEAKS
void RadJavAlloc::logAlloc(void *alloc)
{
	if (alloc == NULL)
		return;

	RadJAV::MemoryAllocLog allocLog(alloc, file, line, func);
	RadJAV::RadJav::logNewMemoryAlloc(allocLog);
}

void RadJavAlloc::logFree(void *alloc)
{
	RadJAV::MemoryAllocLog allocLog(alloc);
	RadJAV::RadJav::removeMemoryAlloc(allocLog);
	free(alloc);
}

void *operator new (size_t size, const char *file, int line, const char *func)
{
	void *alloc = malloc(size);

	//RadJAV::MemoryAllocLog allocLog(alloc, RadJAV::String(file), line, RadJAV::String(func));
	//RadJAV::RadJav::logNewMemoryAlloc(allocLog);

	return (alloc);
}

void *operator new [](size_t size, const char *file, int line, const char *func)
{
	void *alloc = malloc(size);

	//RadJAV::MemoryAllocLog allocLog(alloc, RadJAV::String(file), line, RadJAV::String(func));
	//RadJAV::RadJav::logNewMemoryAlloc(allocLog);

	return (alloc);
}

void *operator new (size_t size) throw (std::bad_alloc)
{
	return (operator new (size, "", -1, ""));
}

void *operator new [](size_t size) throw (std::bad_alloc)
{
	return (operator new [](size, "", -1, ""));
}

void *operator new (size_t size, const std::nothrow_t &) _NOEXCEPT
{
	void *alloc = malloc(size);

	return (alloc);
}

#if __APPLE__
void *operator new [](size_t size, const std::nothrow_t &)  _NOEXCEPT
{
	void *alloc = malloc(size);
	
	return (alloc);
}
#else
void *operator new [](size_t size, const std::nothrow_t &) throw (std::bad_alloc)
{
	void *alloc = malloc(size);

	return (alloc);
}
#endif

void operator delete (void *alloc) _NOEXCEPT
{
	RadJavAlloc::logFree(alloc);
}

void operator delete [](void *alloc) _NOEXCEPT
{
	RadJavAlloc::logFree(alloc);
}

void operator delete (void *alloc, size_t) _NOEXCEPT
{
	RadJavAlloc::logFree(alloc);
}

void operator delete [](void *alloc, size_t) _NOEXCEPT
{
	RadJavAlloc::logFree(alloc);
}

void operator delete (void *alloc, const std::nothrow_t&) _NOEXCEPT
{
	operator delete (alloc, "", -1, "");
}

void operator delete [](void *alloc, const std::nothrow_t&) _NOEXCEPT
{
	operator delete [](alloc, "", -1, "");
}

void operator delete (void *alloc, const char *file, int line, const char *func) _NOEXCEPT
{
	RadJAV::MemoryAllocLog allocLog(alloc);
	RadJAV::RadJav::removeMemoryAlloc(allocLog);
	
	operator delete (alloc);
}

void operator delete [](void *alloc, const char *file, int line, const char *func) _NOEXCEPT
{
	RadJAV::MemoryAllocLog allocLog(alloc);
	RadJAV::RadJav::removeMemoryAlloc(allocLog);

	operator delete [](alloc);
}
#endif
#endif

namespace RadJAV
{
	#ifdef GUI_USE_WXWIDGETS
		wxWidgetsRadJav *RadJav::app = NULL;
	#else
		String _radjav_exec_path = "";
	#endif
	JavascriptEngine *RadJav::javascriptEngine = NULL;
	Theme *RadJav::theme = NULL;
	Lang *RadJav::lang = NULL;
	Array<String> RadJav::arguments;

	#ifdef RADJAV_DEBUG
		HashMap<size_t, MemoryAllocLog> *RadJav::memoryAllocs;
	#endif

	void Pause()
	{
		#ifdef WIN32
			system("PAUSE");
		#endif
	}

		RadJavType RadJav::initialize(Array<String> newArgs, String &file)
		{
			arguments.clear();
			CPP::OS::args.clear();

			#ifndef GUI_USE_WXWIDGETS
				_radjav_exec_path.assign(newArgs[0]);
			#endif
			
			#ifdef RADJAV_DEBUG
				memoryAllocs = new HashMap<size_t, MemoryAllocLog>();
			#endif

			if (newArgs.size() > 0)
			{
				RJBOOL pause = false;
				RJBOOL foundArgs = false;

				for (RJUINT iIdx = 1; iIdx < newArgs.size(); iIdx++)
				{
					String arg = newArgs.at(iIdx);
					RJBOOL found = false;

					if (foundArgs == true)
					{
						arguments.push_back(arg);

						continue;
					}

					CPP::OS::args.push_back(arg);

					if (arg == "--terminal")
					{
						found = true;

						#ifdef WIN32
							setupConsoleOutput();
						#endif
					}

					if (arg == "--xrjv1")
					{
						found = true;

						#ifdef WIN32
							setupConsoleOutput();
						#endif

						#ifdef USE_BLOCKCHAIN_V1
							SetupEnvironment();

							noui_connect();

							int argc = newArgs.size() - iIdx;
							char **argv = new char *[argc];

							for (RJUINT iJdx = iIdx; iJdx < newArgs.size(); iJdx++)
							{
								char *cstr = (char *)newArgs.at(iJdx).c_str();
								RJUINT index = iJdx - iIdx;
								argv[index] = cstr;
							}

							AppInit(argc, argv);

							DELETEARRAY(argv);
						#else
							RadJav::showError("BlockchainV1 was not compiled with this version of RadJav.");
							#ifdef WIN32
								if (pause == false)
									system("PAUSE");
							#endif
						#endif

						if (pause == true)
							Pause();

						return (RadJavType::XRJ_NODE);
					}

					if (arg == "--xrjv1-cli")
					{
						found = true;

						#ifdef WIN32
							setupConsoleOutput();
						#endif

						#ifdef USE_BLOCKCHAIN_V1
							int argc = newArgs.size() - iIdx;
							char **argv = new char *[argc];

							for (RJUINT iJdx = iIdx; iJdx < newArgs.size(); iJdx++)
							{
								char *cstr = (char *)newArgs.at(iJdx).c_str();
								RJUINT index = iJdx - iIdx;
								argv[index] = cstr;
							}

							startXRJV1CLI(argc, argv);

							DELETEARRAY(argv);
						#else
							RadJav::showError("BlockchainV1 was not compiled with this version of RadJav.");
							#ifdef WIN32
								if (pause == false)
									system("PAUSE");
							#endif
						#endif

						if (pause == true)
							Pause();

						return (RadJavType::XRJ_NODE);
					}

					if (arg == "--version")
					{
						found = true;

						#ifdef WIN32
							setupConsoleOutput();
						#endif

						RadJav::printToOutputWindow("RadJav Version " + String (RADJAV_VERSION_MAJOR) + "." + String (RADJAV_VERSION_MINOR) + "\n");
						#ifdef USE_BLOCKCHAIN_V1
							RadJav::printToOutputWindow("XRJV1 " + String(CLIENT_VERSION_MAJOR) + "." + String (CLIENT_VERSION_MINOR) + "." + String (CLIENT_VERSION_REVISION) + "  is included.\n");
						#else
							RadJav::printToOutputWindow("XRJV1 is not included.\n");
						#endif

						if (pause == true)
							Pause();

						return (RadJavType::XRJ_NODE);
					}

					if (arg == "--pause")
					{
						found = true;
						pause = true;
					}

					if (arg == "--help")
					{
						found = true;
						RadJav::printToOutputWindow("RadJav Version " + String(RADJAV_VERSION_MAJOR) + "." + String(RADJAV_VERSION_MINOR) + "\n");
						RadJav::printToOutputWindow("--help Help\n");
						RadJav::printToOutputWindow("--version Version\n");
						RadJav::printToOutputWindow("--pause Pause after execution.\n");
						RadJav::printToOutputWindow("--terminal Output to the terminal.\n");
						RadJav::printToOutputWindow("--xrjv1 Start a XRJV1 node.\n");
						RadJav::printToOutputWindow("--xrjv1-cli Manage a XRJV1 node.\n");


						return (RadJavType::XRJ_NODE);
					}

					if (found == false)
					{
						if (CPP::IO::isFile(arg) == true)
						{
							foundArgs = true;
							file = arg;
						}
					}
				}

				if (pause == true)
					Pause();
			}

			#ifdef GUI_USE_WXWIDGETS
				// Initializing wxWidgets app and lib
				// TODO: not sure what to do if it fails
				int argc = 1;
				char* argv[] = { const_cast<char*>(newArgs.at (0).c_str()) };
				/*int argc = newArgs.size();
				char **argv = RJNEW char *[argc];

				for (RJUINT iIdx = 0; iIdx < newArgs.size(); iIdx++)
					argv[iIdx] = (char *)newArgs.at(iIdx).c_str();*/

				try
				{
					if (wxInitialize(argc, argv))
					{
						wxInitAllImageHandlers();
						app = dynamic_cast<wxWidgetsRadJav*>(wxTheApp);
						app->OnInit();
				}
				}
				catch (Exception ex)
				{
					return (RadJavType::VM);
				}
			#endif
			
			#ifdef HTTP_USE_CIVETWEB
				mg_init_library(8 | 16);
			#endif

			#ifdef HTTP_USE_CURL
				curl_global_init (CURL_GLOBAL_DEFAULT);
			#endif

			theme = RJNEW Theme ();

			#ifdef USE_V8
				javascriptEngine = RJNEW V8JavascriptEngine ();
			#endif

            #ifdef USE_JAVASCRIPTCORE
                javascriptEngine = RJNEW JSCJavascriptEngine ();
            #endif

			return (RadJavType::VM);
		}

	#ifdef WIN32
	void RadJav::setupConsoleOutput()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;

		AllocConsole();

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
		csbInfo.dwSize.Y = 200;
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), csbInfo.dwSize);

		long lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
		int iConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
		FILE *fFile = _fdopen(iConHandle, "w");
		*stdout = *fFile;
		setvbuf(stdout, 0, _IONBF, 0);

		lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
		iConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
		fFile = _fdopen(iConHandle, "r");
		*stdin = *fFile;
		setvbuf(stdin, 0, _IONBF, 0);

		lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
		iConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
		fFile = _fdopen(iConHandle, "w");
		*stderr = *fFile;
		setvbuf(stderr, 0, _IONBF, 0);

		freopen("CONOUT$", "w", stdout);
		std::ios::sync_with_stdio();
	}
	#endif

	int RadJav::runApplication (String application, String fileName)
	{
		if (javascriptEngine != NULL)
			return javascriptEngine->runApplication (application, fileName);
		
		return EXIT_FAILURE;
	}

	int RadJav::runApplicationFromFile (String file)
	{
		if (javascriptEngine != NULL)
			return javascriptEngine->runApplicationFromFile (file);

		return EXIT_FAILURE;
	}

	void RadJav::showMessageBox(String message, String title)
	{
		#ifdef GUI_USE_WXWIDGETS
			//app->CallAfter([&message, &title]()
			//{
				wxMessageBox(message.towxString(), title.towxString(), wxOK);
			//});
		#elif defined USE_IOS || defined USE_ANDROID
			CPP::MUI::AlertFrame::show(title, message);
		#else
			#warning Add Alert(Message Box) implementation
		#endif
	}

	void RadJav::showError(String message, RJBOOL showMessageBox)
	{
		std::cout << message.c_str () << std::endl;
		LOGE("%s: %s", "RadJav::showError", message.c_str());

		if (showMessageBox == true)
			RadJav::showMessageBox(message, "Error");
	}

	void RadJav::throwException(String message)
	{
		if (javascriptEngine != NULL)
			javascriptEngine->throwException(message);
	}

	void RadJav::printToOutputWindow(String message)
	{
		#ifdef WIN32
			#ifdef USE_CHAR
				OutputDebugStringA(message.c_str ());
			#else
				OutputDebugString (message.towcharString ());
			#endif
		#endif

		std::cout << message.c_str ();

		LOGI("%s: %s", "RadJav::printToOutputWindow", message.c_str());
	}

	void RadJav::shutdown()
	{
		DELETEOBJ(javascriptEngine);

		DELETEOBJ (lang);
		DELETEOBJ(theme);

		#ifdef GUI_USE_WXWIDGETS
			wxUninitialize();
		#endif
		
		#ifdef HTTP_USE_CURL
			curl_global_cleanup ();
		#endif

		#if defined (RADJAV_DEBUG) && defined (LOG_MEMORY_LEAKS)
			RadJav::writeMemoryLeaksToFile("./leaks.csv");
		#endif
	}

	#ifdef RADJAV_DEBUG
	void RadJav::logNewMemoryAlloc(MemoryAllocLog alloc)
	{
		if (memoryAllocs == NULL)
			return;

		memoryAllocs->insert (HashMapPair<size_t, MemoryAllocLog> ((size_t)alloc.alloc, alloc));
	}

	void RadJav::removeMemoryAlloc(MemoryAllocLog alloc)
	{
		if (memoryAllocs == NULL)
			return;

		HashMap<size_t, MemoryAllocLog>::iterator it = memoryAllocs->find ((size_t)alloc.alloc);
		HashMap<size_t, MemoryAllocLog>::iterator endAlloc = memoryAllocs->end ();

		if (it != endAlloc)
			memoryAllocs->erase (it);
	}

	String RadJav::reportMemoryLeaks()
	{
		HashMap<size_t, MemoryAllocLog>::iterator begin = memoryAllocs->begin ();
		HashMap<size_t, MemoryAllocLog>::iterator end = memoryAllocs->end ();
		String memoryLeaksReport = "";
		int numLeaks = 0;

		memoryLeaksReport = "Memory Location,File,Line,Function\n";

		while (begin != end)
		{
			MemoryAllocLog alloc = (*begin).second;
			memoryLeaksReport += String::fromUInt((size_t)alloc.alloc) + ",";
			memoryLeaksReport += String (alloc.file) + ",";
			memoryLeaksReport += String::fromInt (alloc.line) + ",";
			memoryLeaksReport += String (alloc.func) + "\n";
			numLeaks++;
			begin++;
		}

		memoryLeaksReport += "\n";
		memoryLeaksReport += "Number of memory leaks: " + numLeaks;

		return (memoryLeaksReport);
	}

	void RadJav::writeMemoryLeaksToFile(String path)
	{
		String leaks = RadJav::reportMemoryLeaks();

		std::fstream file(path.c_str (), std::ios_base::out);
		file.write(leaks.c_str(), leaks.size());
		file.close();
	}
	#endif
//#ifdef __cplusplus
	extern "C" {
//#endif
	//Static c-style interface for RadJav VM
	namespace Static {
		/// Start RadJav.
		RadJavType initializeVM(const char** newArgs, int argSize, char* fileBuffer, int bufferSize) {
			//TODO: add better parameter checks
			assert(argSize >= 0);
			assert(bufferSize >= 0);

			Array<String> args;
			for (int t = 0; t < argSize; ++t) {
				args.push_back(String(newArgs[t]));
			}

			String file;
			file.append(fileBuffer);

			RadJavType returnValue = RadJav::initialize(args, file);

			memset(fileBuffer, '\0', bufferSize);
			strncpy(fileBuffer, file.c_str(), bufferSize);

			return returnValue;
		};

//		RadJavType initializeVM(const char** newArgs, int argSize) {
//			//TODO: add better parameter checks
//			assert(argSize >= 0);
//
//			Array<String> args;
//			for (int t = 0; t < argSize; ++t) {
//				args.push_back(String(newArgs[t]));
//			}
//
//			return RadJav::initialize(args);
//		};

#ifdef WIN32
		/// Setup console output.
		void setupConsoleOutput() {
			return RadJav::setupConsoleOutput();
		};
#endif

		/// Shut down and stop all applications.
		void shutdownVM() {
			return RadJav::shutdown();
		};

		/// Run an application.
		int runApplication(const char* application, const char* fileName) {
			return RadJav::runApplication(String(application), String(fileName));
		};

		/// Run an application from a javascript file.
		int runApplicationFromFile(const char* file) {
			return RadJav::runApplicationFromFile(String(file));
		};

		/// Show a message box.
		void showMessageBox(const char* message, const char* title) {
			return RadJav::showMessageBox(String(message), String(title));
		};

		/// Show an error message.
		void showError(const char* message, bool showMessageBox) {
			RJBOOL flag = (RJBOOL)showMessageBox;
			return RadJav::showError(String(message), flag);
		};

		/// Show an error message.
		void throwException(const char* message) {
			return RadJav::throwException(String(message));
		};

		/// Primarily for Visual Studio debugging. This will print to the output window, or
		/// to the console if on another platform.
		void printToOutputWindow(const char* message) {
			return RadJav::printToOutputWindow(String(message));
		};
	}
//#ifdef __cplusplus
	}
//#endif
}
