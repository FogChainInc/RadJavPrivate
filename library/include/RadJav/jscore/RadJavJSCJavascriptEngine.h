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
#ifndef _RADJAV_JSC_JAVASCRIPT_ENGINE_H_
	#define _RADJAV_JSC_JAVASCRIPT_ENGINE_H_

	#include "RadJavPreprocessor.h"

#ifdef USE_JAVASCRIPTCORE
    #include <JavaScriptCore/JavaScriptCore.h>
    #include <JavaScriptCore/JSObjectRef.h>

	#include <atomic>
	#include <chrono>
	#include <utility>
	#include <vector>

	#include "RadJavJavascriptEngine.h"

	#ifdef GUI_USE_WXWIDGETS
		#include <wx/event.h>
	#endif

	// The USE(x, ...) template is used to silence C++ compiler warnings
	// issued for (yet) unused variables (typically parameters).
	// The arguments are guaranteed to be evaluated from left to right.
	struct Use {
		template <typename T>
		Use(T&&) {}  // NOLINT(runtime/explicit)
	};
	#define USE(...)                                         \
	  do {                                                   \
		::Use unused_tmp_array_for_use_macro[]{__VA_ARGS__}; \
		(void)unused_tmp_array_for_use_macro;                \
	  } while (false)

    #define JSC_CALLBACK(object, functionName, function) \
            { \
                JSStringRef functionNameStr = JSStringCreateWithUTF8CString (functionName); \
                JSObjectRef functionObj = JSObjectMakeFunctionWithCallback (context, functionNameStr, &function); \
                JSObjectSetProperty (context, object, functionNameStr, functionObj, kJSPropertyAttributeNone, NULL); \
                JSStringRelease (functionNameStr); \
            }
    #define JSC_JAVASCRIPT_ENGINE static_cast<JSCJavascriptEngine *> (RadJav::javascriptEngine)
    #define JSC_RADJAV /// @todo Finish this.

	namespace RadJAV
	{
		/// The JavaScriptCore javascript engine.
		class RADJAV_EXPORT JSCJavascriptEngine: public JavascriptEngine
		{
			public:
				JSCJavascriptEngine();
				~JSCJavascriptEngine();

				/// Run an application.
				int runApplication(String applicationSource, String fileName);

				/// Execute single step of application
				bool runApplicationSingleStep();
			
				#ifdef GUI_USE_WXWIDGETS
					void runApplicationInIdleEvent(wxIdleEvent& event);
				#endif
			
				/// Run an application from a javascript file.
				int runApplicationFromFile(String file);
				/// Execute Javascript code.
				void executeScript(Array<String> code, String fileName);
				/// Execute Javascript code.
				void executeScript(String code, String fileName);
				/// Connect the native library to the Javascript library.
				void loadNativeCode();

				/// Destroy the JavaScript objects.
				void destroyJSObjects();

				/// Collect the garbage. This will only work if the engine is started with exposeGC = true.
				void collectGarbage();

				#ifdef C3D_USE_OGRE
					/// Start the 3d engine.
					void start3DEngine();
				#endif

				/// Add a timeout, process it.
				//void addTimeout(v8::Persistent<v8::Function> *func, RJINT time);

				/// A blockchain event has occurred, process it.
				void blockchainEvent(String event, String dataType = "null", void *data = NULL);
				//void blockchainEvent(String event, RJINT numargs, v8::Local<v8::Value> *args, RJBOOL alreadyEnteredCritialSection = false);

				/// Add a thread to be handled by the engine.
				void addThread(Thread *thread);
				/// Remove a thread.
				void removeThread(Thread *thread);
				/// Throw a Javascript exception.
				void throwException(String message);

				/// Shutdown the application entirely.
				void exit(RJINT exitCode);

				// Create a promise.
				/*v8::Local<v8::Object> createPromise(v8::Local<v8::Function> function);
				// Create a promise.
				v8::Local<v8::Object> createPromise(
					v8::Local<v8::Object> context, v8::Local<v8::Function> function, v8::Local<v8::Array> args = v8::Local<v8::Array> ());*/

				/// Load the native templates into the javascript library.
				//static void loadTemplates(const v8::FunctionCallbackInfo<v8::Value> &args);
				/// Run the event loop.
				//static void runEventLoopSingleStep(const v8::FunctionCallbackInfo<v8::Value> &args);

                JSGlobalContextRef globalContext;
                JSObjectRef globalObj;
                JSObjectRef radJav;

				#ifdef GUI_USE_WXWIDGETS
					wxCriticalSection *criticalSection;
				#endif

			protected:
				Array<String> jsToExecuteNextCode;
				Array<String> jsToExecuteNextFilename;
		};
	}
#endif
#endif
