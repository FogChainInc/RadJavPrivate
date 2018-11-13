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
#ifndef _RADJAV_H_
	#define _RADJAV_H_

	#include "RadJavPreprocessor.h"
	#include "RadJavLang.h"
	#include "RadJavString.h"
	#include "RadJavArray.h"
	#include "RadJavHashMap.h"
	#include "RadJavTheme.h"
	#include "RadJavException.h"

	#ifdef RADJAV_DEBUG
		#include "RadJavMemoryAllocLog.h"
	#endif

	#include <vector>
	#include <ostream>

	#ifdef GUI_USE_WXWIDGETS
		#include "RadJavWxWidgets.h"
	#endif

	#include "RadJavJavascriptEngine.h"
	#include "RadJavLang.h"

    #define TEMP_HEADER_INCLUDE
    #include "cpp/RadJavCPPOSScreenInfo.h"
    #undef TEMP_HEADER_INCLUDE

    #ifdef USE_ANDROID
    #include "android/UiThreadCallbackFunction.h"
    #endif

	/// The RadJAV namespace.
	namespace RadJAV
	{
//#ifdef __cplusplus
		extern "C" {
//#endif
			enum RadJavType
			{
				VM = 1,
				XRJ_NODE = 2
			};
//#ifdef __cplusplus
		}
//#endif
        #ifdef USE_ANDROID
            class RadJavAndroid;
        #endif

	    #ifndef GUI_USE_WXWIDGETS
		    extern String _radjav_exec_path;
        #endif

		/// The main RadJav class, this class handles most if not all functionality.
		class RADJAV_EXPORT RadJav
		{
			public:
				#ifdef GUI_USE_WXWIDGETS
					/// Get the wxWidgets system.
					inline static wxWidgetsRadJav *getwxWidgetsApp()
					{
						return (RadJav::app);
					}
				#endif

				/// Start RadJav.
				static RadJavType initialize(Array<String> newArgs, String &file);

				#ifdef USE_ANDROID
					static RadJavType initialize(JavaVM* jvm);
                #endif

				#ifdef WIN32
					/// Setup console output.
					static void setupConsoleOutput();
				#endif

				/// Shut down and stop all applications.
				static void shutdown();

                /// Setup the screen info.
                static void setupScreens ();

				#ifdef USE_CRYPTOGRAPHY
					/// Setup the Crypto library.
					static void setupCrypto ();
				#endif

				/// Run an application.
				static int runApplication (String application, String fileName);
				/// Run an application from a javascript file.
				static int runApplicationFromFile (String file);

				/// Show a message box.
				static void showMessageBox(String message, String title);

				/// Show an error message.
				static void showError (String message, RJBOOL showMessageBox = false);

				/// Show an error message.
				static void throwException(String message);

				/// Primarily for Visual Studio debugging. This will print to the output window, or 
				/// to the console if on another platform.
				static void printToOutputWindow (String message);

				/// Add a thread for processing.
				static void addThread(Thread *thread);
				/// Remove a thread from processing.
				static void removeThread(Thread *thread);

                /// Screen info attached to this device.
                static Array<RadJAV::CPP::OS::ScreenInfo> screens;

				/// Handles strings from languages.
				static Lang *lang;

				/// Handles the loaded Theme.
				static Theme *theme;

				#ifdef GUI_USE_WXWIDGETS
					/// The GUI system used to display the GUI.
					static wxWidgetsRadJav *app;
				#endif

				/// The javascript engine used to execute applications.
				static JavascriptEngine *javascriptEngine;
				/// The command line arguments used for the XRJ application.
				static Array<String> arguments;
				#ifdef RADJAV_DEBUG
					static void logNewMemoryAlloc (MemoryAllocLog alloc);
					static void removeMemoryAlloc (MemoryAllocLog alloc);
					static String reportMemoryLeaks();
					static void writeMemoryLeaksToFile(String path);

					/// Memory allocations made during debug.
					static HashMap<size_t, MemoryAllocLog> *memoryAllocs;
                #endif

			#ifdef USE_ANDROID
		    public:
                ///Request function execution on Java UI thread
                static void runOnUiThreadAsync(UiThreadCallbackFunctionType function, void *data);

                ///Get main java application instance
                static jobject getJavaApplication();

                ///Get main java ViewGroup
                static jobject getJavaViewGroup();

                static bool isWaitingForUiThread();

                static bool isPaused();

			private:
		        static RadJavAndroid* impl;
			#endif
		};
//#ifdef __cplusplus
		extern "C" {
//#endif
			//Static c-style interface for RadJav VM
			namespace Static {
				/// Start RadJav.
				RADJAV_EXPORT RadJavType initializeVM(const char** newArgs, int argSize, char* fileBuffer, int bufferSize);
				//RADJAV_EXPORT RadJavType initializeVM(const char** newArgs, int argSize);
#ifdef WIN32
				/// Setup console output.
				RADJAV_EXPORT void setupConsoleOutput();
#endif

				/// Shut down and stop all applications.
				RADJAV_EXPORT void shutdownVM();

                /// Setup the screen info.
                RADJAV_EXPORT void setupScreens ();

				#ifdef USE_CRYPTOGRAPHY
					/// Setup the Crypto library.
					RADJAV_EXPORT void setupCrypto();
				#endif

				/// Run an application.
				RADJAV_EXPORT int runApplication(const char* application, const char* fileName);
				/// Run an application from a javascript file.
				RADJAV_EXPORT int runApplicationFromFile(const char* file);

				/// Show a message box.
				RADJAV_EXPORT void showMessageBox(const char* message, const char* title);

				/// Show an error message.
				RADJAV_EXPORT void showError(const char* message, bool showMessageBox = false);

				/// Show an error message.
				RADJAV_EXPORT void throwException(const char* message);

				/// Primarily for Visual Studio debugging. This will print to the output window, or
				/// to the console if on another platform.
				RADJAV_EXPORT void printToOutputWindow(const char* message);
			}

//#ifdef __cplusplus
		}
//#endif
	}
#endif

