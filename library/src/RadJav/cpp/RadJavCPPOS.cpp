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

		RJINT OS::exec(String command)
		{
			//std::future<std::string> output;
			//RJINT exitCode = boost::process::system (command.c_str (), boost::process::std_out > output);
			//String result = output.get();

			std::error_code ec;
			RJINT exitCode = boost::process::system(command.c_str (), ec);
			
			return exitCode;
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

		#if not defined USE_IOS && not defined USE_ANDROID
		class OS::SystemProcessImpl : public std::enable_shared_from_this<OS::SystemProcessImpl>
		{
		public:
			SystemProcessImpl(ContextManager& contextManager)
			: _contextManager(contextManager)
			  , _context(_contextManager.getContext())
			  , _pipe(_context)
			{
				_exitCode = 0;
				_child = nullptr;
			}
			
			~SystemProcessImpl()
			{
				if (_child)
				{
					DELETEOBJ(_child);
					_child = nullptr;
					
					if (_ec)
					{
						if (_onError)
							_onError(_ec.value(), _ec.message());
					}
					else if (_onExit)
					{
						_contextManager.contextReleased(_context);
						_onExit(_exitCode);
					}
				}
			}
			
			void start(const std::string& command,
					   const std::vector<std::string>& arguments,
					   std::function<void(RJINT, const String&)> onError,
					   std::function<void(const String&)> onOutput,
					   std::function<void(RJINT)> onExit)
			{
				if (_child)
					return;

				_onError = onError;
				_onOutput = onOutput;
				_onExit = onExit;

				std::error_code ec;

				namespace process = boost::process;
				_child = RJNEW process::child(command.c_str(),
											  arguments,
											  (process::std_err & process::std_out) > _pipe,
											  process::on_exit(std::bind(&SystemProcessImpl::internalOnExit,
																		 shared_from_this(),
																		 std::placeholders::_1,
																		 std::placeholders::_2)),
											  _context,
											  ec);

				if (ec)
				{
					_ec = ec;
					return;
				}

				doRead();

				_contextManager.activateContext(_context);
				
			}
			
			void terminate()
			{
				if (_child && _child->running())
				{
					_child->terminate();
				}
			}

			void terminateSilently()
			{
				if (_child && _child->running())
				{
					_onExit = nullptr;
					_onOutput = nullptr;
					_onError = nullptr;

					_child->terminate();
				}
			}

		private:
			void doRead()
			{
				boost::asio::async_read(_pipe,
										_output,
										std::bind(&SystemProcessImpl::internalOnRead,
												  shared_from_this(),
												  std::placeholders::_1,
												  std::placeholders::_2));
				
			}
			
			void internalOnRead(const boost::system::error_code& ec, std::size_t bytes_read)
			{
				if (bytes_read)
				{
					std::istream istream(&_output);
					std::stringstream output;
					istream>>output.rdbuf();
					
					if (_onOutput)
					{
						_onOutput(output.str());
					}
					
					_output.consume(bytes_read);
				}

				if (!ec)
					doRead();
			}
			
			void internalOnExit(int exitCode, const std::error_code&)
			{
				_exitCode = exitCode;
			}
			
		private:
			std::function<void(RJINT, const String&)> _onError;
			std::function<void(const String&)> _onOutput;
			std::function<void(RJINT)> _onExit;
			
			ContextManager& _contextManager;
			boost::asio::io_context& _context;
			boost::process::async_pipe _pipe;
			boost::process::child *_child;
			boost::asio::streambuf _output;
			std::string _command;
			std::error_code _ec;
			RJINT _exitCode;
		};
		
		OS::SystemProcess::SystemProcess(const String& command,
										 ContextManager& contextManager)
		: _contextManager(contextManager)
		{
			this->command = command;
		}

		OS::SystemProcess::SystemProcess(const String& command,
										 const Array<String>& args,
										 ContextManager& contextManager)
		: _contextManager(contextManager)
		{
			this->command = command;
			this->args = args;
		}

		#ifdef USE_V8
			OS::SystemProcess::SystemProcess(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
			: _contextManager(*jsEngine->contextManager)
			{
				command = V8_JAVASCRIPT_ENGINE->v8GetString(args.This(), "command");
				v8::Local<v8::Object> argsObj = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "args");
				v8::Local<v8::Array> argsAry = v8::Local<v8::Array>::Cast (argsObj);

				for (RJUINT iIdx = 0; iIdx < argsAry->Length(); iIdx++)
				{
					v8::Local<v8::String> str = v8::Local<v8::String>::Cast (argsAry->Get(iIdx));

					this->args.push_back(parseV8Value (str));
				}
			}
		#elif defined USE_JAVASCRIPTCORE
			OS::SystemProcess::SystemProcess(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[])
			: _contextManager(*jsEngine->contextManager)
			{
				command = JSC_JAVASCRIPT_ENGINE->jscGetString(thisObject, "command");
				JSObjectRef argsObjJs = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "args");
				
				if (JSValueIsArray(jsEngine->globalContext, argsObjJs))
				{
					RJINT argsSize = JSC_JAVASCRIPT_ENGINE->jscGetInt(argsObjJs, "length");
					
					for (RJINT iIdx = 0; iIdx < argsSize; iIdx++)
					{
						JSValueRef argJs = JSObjectGetPropertyAtIndex(jsEngine->globalContext, argsObjJs, iIdx, nullptr);
						args.push_back(parseJSCValue(jsEngine->globalContext, argJs));
					}
				}
			}
		#endif

		OS::SystemProcess::~SystemProcess()
		{
			auto impl = _impl.lock();
			if (impl)
				impl->terminateSilently();
			
			// Events designed mostly for GUI and they will be freed by GUI library
			// but in our case where we are not relying on GUI here we need to remove events manually
			for(auto it = events->begin(); it != events->end(); it++)
			{
				DELETEOBJ(it->second);
			}
		}

		void OS::SystemProcess::execute()
		{
			if (_impl.lock())
				return;
			
			auto impl = std::make_shared<SystemProcessImpl>(_contextManager);

			_impl = impl;

			std::vector<std::string> tempArgs;
			for (RJUINT iIdx = 0; iIdx < args.size(); iIdx++)
				tempArgs.push_back(args.at (iIdx));

			impl->start(command, tempArgs,
						std::bind(&SystemProcess::internalOnError, this,
								  std::placeholders::_1,
								  std::placeholders::_2),
						std::bind(&SystemProcess::internalOnOutput, this,
								  std::placeholders::_1),
						std::bind(&SystemProcess::internalOnExit, this,
								  std::placeholders::_1));
		}

		void OS::SystemProcess::kill()
		{
			auto impl = _impl.lock();
			if (impl)
			{
				impl->terminate();
			}
		}
		
		void OS::SystemProcess::internalOnError(RJINT errorCode, const String& description)
		{
			// If we use SystemProcess from C++ side
			// Then process only C++ callbacks
			if (_onError)
			{
				_onError(errorCode, description);
				return;
			}
			
			#ifdef USE_V8
				v8::Local<v8::Value> args[2];
				args[0] = v8::Number::New(V8_JAVASCRIPT_ENGINE->isolate, errorCode);
				args[1] = String(description).toV8String(V8_JAVASCRIPT_ENGINE->isolate);
			
				executeEvent("error", 2, args);
			
			#elif defined USE_JAVASCRIPTCORE
				JSValueRef args[2];
				args[0] = JSValueMakeNumber(JSC_JAVASCRIPT_ENGINE->globalContext, errorCode);
				args[1] = String(description).toJSCValue(JSC_JAVASCRIPT_ENGINE->globalContext);
			
				executeEvent("error", 2, args);
			#endif
		}
		
		void OS::SystemProcess::internalOnOutput(const String& output)
		{
			// If we use SystemProcess from C++ side
			// Then process only C++ callbacks
			if (_onOutput)
			{
				_onOutput(output);
				return;
			}
			
			#ifdef USE_V8
				v8::Local<v8::Value> args[1];
				args[0] = String(output).toV8String(V8_JAVASCRIPT_ENGINE->isolate);
			
				executeEvent("output", 1, args);
			
			#elif defined USE_JAVASCRIPTCORE
				JSValueRef args[1];
				args[0] = String(output).toJSCValue(JSC_JAVASCRIPT_ENGINE->globalContext);
			
				executeEvent("output", 1, args);
			#endif
		}
		
		void OS::SystemProcess::internalOnExit(RJINT exitCode)
		{
			// If we use SystemProcess from C++ side
			// Then process only C++ callbacks
			if (_onExit)
			{
				_onExit(exitCode);
				return;
			}
			
			#ifdef USE_V8
				v8::Local<v8::Value> args[1];
				args[0] = v8::Number::New(V8_JAVASCRIPT_ENGINE->isolate, exitCode);
			
				executeEvent("exit", 1, args);
			
			#elif defined USE_JAVASCRIPTCORE
				JSValueRef args[1];
				args[0] = JSValueMakeNumber(JSC_JAVASCRIPT_ENGINE->globalContext, exitCode);
			
				executeEvent("exit", 1, args);
			#endif
		}
		
		#if defined USE_V8 || defined USE_JAVASCRIPTCORE
			void OS::SystemProcess::on(String event, RJ_FUNC_TYPE func)
			{
				createEvent(event, func);
			}
		#endif
		#endif

	}
}

