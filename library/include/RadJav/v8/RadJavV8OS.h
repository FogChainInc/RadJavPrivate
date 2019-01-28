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
#ifndef _RADJAV_V8_OS_H_
	#define _RADJAV_V8_OS_H_

	#include "RadJavPreprocessor.h"

	#include <v8.h>

	namespace RadJAV
	{
		namespace V8B
		{
			/**
			 * @ingroup group_os_js_v8
			 * @brief OS callbacks.
			 * @details Class representing Javascript bindings to CPP::OS::OS.
			 */
			namespace OS
			{
				void createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object);
				void destroy();

				void onReady(const v8::FunctionCallbackInfo<v8::Value> &args);
				void openWebBrowserURL(const v8::FunctionCallbackInfo<v8::Value> &args);
				void exec(const v8::FunctionCallbackInfo<v8::Value> &args);
				void getDocumentsPath(const v8::FunctionCallbackInfo<v8::Value> &args);
				void getTempPath(const v8::FunctionCallbackInfo<v8::Value> &args);
				void getUserDataPath(const v8::FunctionCallbackInfo<v8::Value> &args);
				void getApplicationPath(const v8::FunctionCallbackInfo<v8::Value> &args);
				void getCurrentWorkingPath(const v8::FunctionCallbackInfo<v8::Value> &args);
				void setCurrentWorkingPath(const v8::FunctionCallbackInfo<v8::Value> &args);
				void saveFileAs(const v8::FunctionCallbackInfo<v8::Value> &args);
				void openFileAs(const v8::FunctionCallbackInfo<v8::Value> &args);

				extern v8::Persistent<v8::Value> *onReadyFunction;
			};
		}
	}

#endif

