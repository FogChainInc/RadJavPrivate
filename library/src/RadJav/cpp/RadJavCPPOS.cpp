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
#include "cpp/RadJavCPPOS.h"

#include "RadJav.h"
#include "RadJavString.h"
#import "cpp/ios/RadJavCPPMUIUtils.h"


#ifdef GUI_USE_WXWIDGETS
	#include <wx/wx.h>
	#include <wx/stdpaths.h>
#endif

namespace RadJAV
{
	namespace CPP
	{
		#ifdef WIN32
			String OS::type = "windows";

			#ifdef _WIN64
				RJINT OS::numBits = 64;
			#else
				RJINT OS::numBits = 32;
			#endif
		#endif
		#if defined (LINUX) && !defined (__APPLE__)
			String OS::type = "linux";

			#ifdef __LP64__
				RJINT OS::numBits = 64;
			#else
				RJINT OS::numBits = 32;
			#endif
		#endif
		#ifdef __APPLE__
            #include <TargetConditionals.h>

			#if TARGET_OS_OSX == 1
				String OS::type = "mac";
				RJINT OS::numBits = 64;
			#endif

            #if TARGET_OS_IOS == 1
                String OS::type = "ios";
                RJINT OS::numBits = 64;
            #endif
		#endif
		Array<String> OS::args;
		std::function<void()> *OS::onReadyFunction = NULL;

		void OS::destroy()
		{
			DELETEOBJ(onReadyFunction);
		}

		void OS::onReady(std::function<void()> asyncCallback)
		{
			onReadyFunction = RJNEW std::function<void()>(asyncCallback);
		}

		RJINT OS::exec(String command)
		{
			RJINT output = 0;

			#ifdef GUI_USE_WXWIDGETS
				output = wxExecute(command.towxString());
			#endif

			return (output);
		}

		String OS::getDocumentsPath()
		{
			String str = "";

			#ifdef GUI_USE_WXWIDGETS
				str = parsewxString(wxStandardPaths::Get().GetDocumentsDir());
			#endif
            
            #ifdef USE_IOS
            str = RadJavApplicationDocumentsDirectory();
            #endif

			return (str);
		}

		String OS::getTempPath()
		{
			String str = "";

			#ifdef GUI_USE_WXWIDGETS
				str = parsewxString(wxStandardPaths::Get().GetTempDir());
			#endif
            
            #ifdef USE_IOS
                str = RadJavApplicationTempDirectory();
            #endif
            
			return (str);
		}

		String OS::getUserDataPath()
		{
			String str = "";

			#ifdef GUI_USE_WXWIDGETS
				str = parsewxString(wxStandardPaths::Get().GetUserConfigDir());
			#endif

			return (str);
		}

		String OS::getApplicationPath()
		{
			String str = "";

			#ifdef GUI_USE_WXWIDGETS
				str = parsewxString(wxStandardPaths::Get().GetExecutablePath());
			#endif
            #ifdef USE_IOS
                str = RadJavApplicationDirectory();
            #endif
			return (str);
		}

		String OS::getCurrentWorkingPath()
		{
			String str = "";

			#ifdef GUI_USE_WXWIDGETS
				str = parsewxString(wxGetCwd());
			#endif

			return (str);
		}

		void OS::setCurrentWorkingPath (String path)
		{
			#ifdef GUI_USE_WXWIDGETS
				wxSetWorkingDirectory(path);
			#endif
		}

		void OS::openWebBrowserURL(String url)
		{
			#ifdef GUI_USE_WXWIDGETS
				wxLaunchDefaultBrowser(url.towxString());
			#endif
		}

		String OS::saveFileAs(String message, String defaultDir, String defaultFile, String wildcard, RJBOOL overwritePrompt)
		{
			String path = "";

			#ifdef GUI_USE_WXWIDGETS
				wxString wxmsg = wxFileSelectorPromptStr;

				if (message != "")
					wxmsg = message.towxString();

				RJLONG style = wxFD_SAVE;

				if (overwritePrompt == true)
					style |= wxFD_OVERWRITE_PROMPT;

				wxFileDialog fileDialog(NULL, wxmsg, defaultDir.towxString(), defaultFile.towxString(), wildcard.towxString(), style);

				if (fileDialog.ShowModal() != wxID_CANCEL)
					path = parsewxString(fileDialog.GetPath());
			#endif

			return (path);
		}

		String OS::openFileAs(String message, String defaultDir, String defaultFile, String wildcard, RJBOOL fileMustExist)
		{
			String path = "";

			#ifdef GUI_USE_WXWIDGETS
				wxString wxmsg = wxFileSelectorPromptStr;

				if (message != "")
					wxmsg = message.towxString();

				RJLONG style = wxFD_OPEN;

				if (fileMustExist == true)
					style |= wxFD_FILE_MUST_EXIST;

				wxFileDialog fileDialog(NULL, wxmsg, defaultDir.towxString(), defaultFile.towxString(), wildcard.towxString(), style);

				if (fileDialog.ShowModal() != wxID_CANCEL)
					path = parsewxString(fileDialog.GetPath());
			#endif

			return (path);
		}
	}
}

