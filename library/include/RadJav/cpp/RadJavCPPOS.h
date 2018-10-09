/*
	MIT-LICENSE
	Copyright (c) 2017-2018 Higher Edge Software, LLC

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
#ifndef _RADJAV_CPP_OS_H_
	#define _RADJAV_CPP_OS_H_

	#include "RadJavPreprocessor.h"
	#include "RadJavString.h"
	#include "RadJavArray.h"
	#include "RadJavHashMap.h"

	#include <functional>

	namespace RadJAV
	{
		namespace CPP
		{
			namespace OS
			{
                /** Represents the current type of operating system.
                * Can be:
                * * windows
                * * linux
                * * macosx
                * * html5
                */
                extern String type;
                /** The number of bits this operating system is.
                */
                extern RJINT numBits;
                /** The command line arguments.
                */
                extern Array<String> args;
                /// The onReady callback to be fired.
                extern std::function<void()> *onReadyFunction;

                /// Destroy any OS objects.
                void destroy();

                /// Get the path to the user's documents folder.
                void onReady(std::function<void()> asyncCallback);
                /// Get the path to the user's documents folder.
                RJINT exec(String command);
                /// Get the path to the user's documents folder.
                String getDocumentsPath();
                /// Get the path to the user's temporary files folder.
                String getTempPath();
                /// Get the path to the user's data files folder.
                String getUserDataPath();
                /// Get the path to the application.
                String getApplicationPath();
                /// Get the current working path.
                String getCurrentWorkingPath();
                /// Set the current working path.
                void setCurrentWorkingPath(String path);
                /// Open a URL in the default web browser.
                void openWebBrowserURL(String url);
                /// Display a save file as dialog.
                String saveFileAs(String message = "", String defaultDir = "", String defaultFile = "", String wildcard = "", RJBOOL overwritePrompt = true);
                /// Display a open file as dialog.
                String openFileAs(String message = "", String defaultDir = "", String defaultFile = "", String wildcard = "", RJBOOL fileMustExist = true);
			};
		}
	}
#endif

