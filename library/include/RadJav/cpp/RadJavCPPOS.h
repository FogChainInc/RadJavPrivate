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
	#include "RadJavCPPContextManager.h"
	#include "RadJavCPPEvent.h"

	#include <functional>

	#ifdef USE_V8
		#include "v8/RadJavV8JavascriptEngine.h"
	#endif

	#ifdef USE_JAVASCRIPTCORE
		#include "jscore/RadJavJSCJavascriptEngine.h"
	#endif

	#include <boost/process.hpp>

	namespace RadJAV
	{
		namespace CPP
		{
			namespace OS
			{
				#if not defined USE_IOS && not defined USE_ANDROID
				class SystemProcessImpl;

				/// Handles subprocesses started by RadJav.
				class RADJAV_EXPORT SystemProcess: public Events
				{
					public:
						SystemProcess(const String& command,
									  ContextManager& contextManager);
						SystemProcess(const String& command,
									  const Array<String>& args,
									  ContextManager& contextManager);
						#ifdef USE_V8
							SystemProcess(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
						#elif defined USE_JAVASCRIPTCORE
							SystemProcess(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[]);
						#endif
						~SystemProcess();

						/// Execute the process.
						void execute();

						/// Kill the process.
						void kill();
					
						void onError(std::function<void(RJINT, const String&)> func);
						void onOutput(std::function<void(const String&)> func);
						void onExit(std::function<void(RJINT)> func);

						/**
					 	* @brief Register Javascript callback
					 	* @details Set the Javascript handler function for specific notification
					 	* @param[in] event name of the event. Can be "error", "output" and "exit"
					 	* @param[in] func Javascript callback function
					 	*/
						#if defined USE_V8 || defined USE_JAVASCRIPTCORE
							void on(String event, RJ_FUNC_TYPE func);
						#endif

					private:
						void internalOnError(RJINT code, const String& description);
						void internalOnOutput(const String& output);
						void internalOnExit(RJINT exitCode);

					private:
						/// The command to execute.
						String command;
						/// The arguments to execute with the command.
						Array<String> args;
						/// The exit code from the command. -1 means the command could not be executed.
						//RJINT exitCode;
						/// The buffer size to get from the output.
						//RJINT bufferSize;
						/// The collected output.
						//String output;

						ContextManager& _contextManager;

						std::weak_ptr<SystemProcessImpl> _impl;
					
						std::function<void(const String&)> _onOutput;
						std::function<void(RJINT, const String&)> _onError;
						std::function<void(RJINT)> _onExit;
				};
				#endif

				/**
				 * @ingroup group_os_cpp
				 * @brief OS routines.
				 * @{
				 */

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

				/// Sleep for x number of milliseconds.
				void sleep(RJINT milliseconds);

				/// Search for a argument passed from the terminal.
				RJINT searchArgs(String key);

                /// Get the path to the user's documents folder.
                void onReady(std::function<void()> asyncCallback);
                /// Execute a system command.
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
                
                /** @} */
			};
		}
	}
#endif

