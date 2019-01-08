/*
 MIT-LICENSE
 Copyright (c) 2018 Higher Edge Software, LLC

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
#ifndef _RADJAV_UITHREADCALLBACKFUNCTION_H
#define _RADJAV_UITHREADCALLBACKFUNCTION_H

#include "Jni.h"
#include <functional>

namespace RadJAV
{
	namespace Android
	{
		class UiThreadDispatcher;
		class AppInterface;

		typedef std::function<void (JNIEnv* env, void* data)> UiThreadCallbackFunctionType;

		class UiThreadCallbackFunction
		{
		public:
			UiThreadCallbackFunction() = delete;
			UiThreadCallbackFunction(const UiThreadCallbackFunction&) = delete;

			UiThreadCallbackFunction(UiThreadDispatcher* dispatcher,
									 UiThreadCallbackFunctionType function, void* data,
									 bool asynchronous = true);

			void dispatch();

			void operator ()(JNIEnv* env);

		private:
			UiThreadDispatcher* _dispatcher;
			UiThreadCallbackFunctionType _function;
			void* _data;
			bool _asynchronous;
		};
	}
}

#endif //RADJAV_UITHREADCALLBACKFUNCTION_H
