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

#ifndef WIN32
	#include <unistd.h>
#endif

#ifdef USE_IOS
	#import "cpp/ios/RadJavCPPMUIUtils.h"
#endif

#ifdef GUI_USE_WXWIDGETS
	#include <wx/wx.h>
	#include <wx/stdpaths.h>
#endif

#include <boost/process.hpp>
#include <boost/process/async.hpp>

#include <boost/asio/io_service.hpp>

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

		void OS::sleep(RJINT milliseconds)
		{
			#ifdef WIN32
				Sleep(milliseconds);
			#else
				usleep(milliseconds * 1000);
			#endif
		}

		RJINT OS::searchArgs(String key)
		{
			RJINT foundIndex = -1;

			for (RJUINT iIdx = 0; iIdx < OS::args.size (); iIdx++)
			{
				String arg = OS::args.at (iIdx);

				if (arg == key)
				{
					foundIndex = iIdx;

					break;
				}

				if (arg == ("--" + key))
				{
					foundIndex = iIdx;

					break;
				}
			}

			return (foundIndex);
		}

		void OS::onReady(std::function<void()> asyncCallback)
		{
			onReadyFunction = RJNEW std::function<void()>(asyncCallback);
		}

		CPP::OS::SystemProcess *OS::exec(String command)
		{
			std::future<std::string> output;
			RJINT exitCode = boost::process::system (command.c_str (), boost::process::std_out > output);
			String result = output.get();

			CPP::OS::SystemProcess *process = RJNEW CPP::OS::SystemProcess(command);
			process->exitCode = exitCode;
			process->output = result;

			return (process);
		}

		void OS::exec(CPP::OS::SystemProcess *process)
		{
			process->execute();
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

		OS::SystemProcess::SystemProcess(String command)
		{
			this->command = command;
			bufferSize = 4096;

			exitCode = -1;
			output = "";
		}

		OS::SystemProcess::SystemProcess(String command, Array<String> args)
		{
			this->command = command;
			this->args = args;
			bufferSize = 4096;

			exitCode = -1;
			output = "";
		}

		#ifdef USE_V8
			OS::SystemProcess::SystemProcess(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				command = V8_JAVASCRIPT_ENGINE->v8GetString(args.This(), "command");
				v8::Local<v8::Object> argsObj = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "args");
				v8::Local<v8::Array> argsAry = v8::Local<v8::Array>::Cast (argsObj);

				for (RJUINT iIdx = 0; iIdx < argsAry->Length(); iIdx++)
				{
					v8::Local<v8::String> str = v8::Local<v8::String>::Cast (argsAry->Get(iIdx));

					this->args.push_back(parseV8Value (str));
				}

				exitCode = V8_JAVASCRIPT_ENGINE->v8GetDecimal(args.This(), "exitCode");
				bufferSize = V8_JAVASCRIPT_ENGINE->v8GetDecimal(args.This(), "bufferSize");
				output = V8_JAVASCRIPT_ENGINE->v8GetString(args.This(), "output");
			}

			v8::Local<v8::Object> OS::SystemProcess::toV8Object(v8::Isolate *isolate)
			{
				v8::Local<v8::Object> v8obj = V8_JAVASCRIPT_ENGINE->v8CreateNewObject("RadJav.OS.SystemProcess");

				v8obj->Set(String ("command").toV8String (isolate), command.toV8String (isolate));

				v8::Local<v8::Array> argsAry = v8::Array::New (isolate);

				for (RJUINT iIdx = 0; iIdx < args.length(); iIdx++)
				{
					v8::Local<v8::String> str = args.at (iIdx).toV8String(isolate);

					argsAry->Set(iIdx, str);
				}

				v8obj->Set(String("command").toV8String(isolate), argsAry);

				v8obj->Set(String("exitCode").toV8String(isolate), v8::Integer::New (isolate, exitCode));
				v8obj->Set(String("bufferSize").toV8String(isolate), v8::Integer::New(isolate, bufferSize));
				v8obj->Set(String("output").toV8String(isolate), output.toV8String(isolate));

				return (v8obj);
			}
		#elif defined USE_JAVASCRIPTCORE
			OS::SystemProcess::SystemProcess(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[])
			{
				/// @todo Fill this out.
			}

			JSObjectRef OS::SystemProcess::toJSCObject();
			{
				/// @todo Fill this out.

				return (null);
			}
		#endif

		void OS::SystemProcess::execute()
		{
			boost::asio::io_service ios;
			boost::process::opstream in;
			boost::process::async_pipe output(ios);
			std::vector<std::string> tempArgs;
			Array<RJCHAR> buffer(bufferSize);

			for (RJUINT iIdx = 0; iIdx < args.size(); iIdx++)
				tempArgs.push_back(args.at (iIdx));

			boost::process::child child(command.c_str (), tempArgs,
				boost::process::std_in < in, 
				boost::process::std_out > output, 
				//boost::process::std_err > output, ios, 
				boost::process::on_exit([&](int, const std::error_code&)
					{
						output.close();
					}));
			boost::asio::async_read(output, boost::asio::buffer(buffer), 
				[&](const boost::system::error_code &err, std::size_t size)
					{
						String str (buffer.begin(), buffer.end());
						onOutput (str);
					});

			ios.run();
		}
	}
}

