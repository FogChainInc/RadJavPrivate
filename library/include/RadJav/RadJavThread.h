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
#ifndef _RADJAV_THREAD_H_
	#define _RADJAV_THREAD_H_

	#include "RadJavPreprocessor.h"

	#ifdef GUI_USE_WXWIDGETS
		#include <wx/thread.h>
		#define LOCK_GUARD(x) wxMutexLocker guard(x)
	#else
		#include <mutex>
		#define LOCK_GUARD(x) std::lock_guard<std::mutex> guard(x)
	#endif

	#ifdef USE_V8
		#include <v8.h>
	#endif

	#include <functional>

	namespace RadJAV
	{
		#ifdef USE_V8
			class V8JavascriptEngine;
		#endif

		/// The thread class.
		#ifdef GUI_USE_WXWIDGETS
			/// Basic threading class.
			class RADJAV_EXPORT Thread: public wxThread
			{
				public:
					Thread();

					/// Set whether or not this thread has started executing.
					/// This is mostly for use in the main app loop.
					inline void setAsStarted(RJBOOL started)
					{
						hasStarted = started;
					}

					/// Check whether or not this thread has started executing.
					inline RJBOOL hasThreadStarted()
					{
						return (hasStarted);
					}

				protected:
					RJBOOL hasStarted;
			};
		#else
			class Thread;
		#endif

		/// Create a thread.
		class RADJAV_EXPORT SimpleThread : public Thread
		{
			public:
				SimpleThread();

				#ifdef GUI_USE_WXWIDGETS
					wxThread::ExitCode Entry();
				#endif

				std::function<void()> onStart;
				std::function<void()> onComplete;
		};

		/// Create a thread that is also a promise.
		class RADJAV_EXPORT PromiseThread : public SimpleThread
		{
			public:
				PromiseThread();

				#ifdef USE_V8
					/// Create a V8 promise.
					v8::Local<v8::Object> createV8Promise(V8JavascriptEngine *engine, v8::Local<v8::Object> context);

					/// Set arguments for when a resolve occurs.
					void setResolveArgs(v8::Isolate *isolate, v8::Local<v8::Array> args);
					/// Set arguments for when a resolve occurs. The persistent will be deleted when the resolve is called.
					inline void setResolveArgs(v8::Persistent<v8::Array> *args)
					{
						resolveArgs = args;
					}

					/// Set arguments for when a resolve occurs.
					void setRejectArgs(v8::Isolate *isolate, v8::Local<v8::Array> args);
					/// Set arguments for when a resolve occurs. The persistent will be deleted when the resolve is called.
					inline void setRejectArgs(v8::Persistent<v8::Array> *args)
					{
						rejectArgs = args;
					}
				#endif

				/// Resolve the promise when necessary.
				void resolvePromise();
				/// Reject the promise when the code in the thread has failed.
				void rejectPromise();

			protected:
				#ifdef USE_V8
					/// Run the promise function and save the resolve and reject funtions.
					static void runPromise(const v8::FunctionCallbackInfo<v8::Value> &args);

					V8JavascriptEngine *engine;
					v8::Persistent<v8::Function> *resolvep;
					v8::Persistent<v8::Function> *rejectp;

					v8::Persistent<v8::Array> *resolveArgs;
					v8::Persistent<v8::Array> *rejectArgs;
				#endif
		};
	}
#endif

