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
	#include "RadJavJSExternals.h"
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
    #define JSC_CCALLBACK(context, object, functionName, function) \
            { \
            JSStringRef functionNameStr = JSStringCreateWithUTF8CString (functionName); \
            JSObjectRef functionObj = JSObjectMakeFunctionWithCallback (context, functionNameStr, &function); \
            JSObjectSetProperty (context, object, functionNameStr, functionObj, kJSPropertyAttributeNone, NULL); \
            JSStringRelease (functionNameStr); \
            }
    #define JSC_JAVASCRIPT_ENGINE static_cast<JSCJavascriptEngine *> (RadJav::javascriptEngine)
    #define JSC_RADJAV JSC_JAVASCRIPT_ENGINE->radJav

	namespace RadJAV
	{
        class JSCJavascriptEngine;
        
        /// Create an asynchronous function call.
        class RADJAV_EXPORT AsyncFunctionCall
        {
            public:
                AsyncFunctionCall(JSObjectRef newfunc, JSObjectRef newargs = NULL, RJBOOL newDeleteOnComplete = true);
                AsyncFunctionCall(JSObjectRef newfunc, RJINT numArgs, JSValueRef *newargs = NULL, RJBOOL newDeleteOnComplete = true);
                ~AsyncFunctionCall();

                /// Check to see if an asynchronous function call has a result. Be sure to set deleteOnComplete = true;
                inline RJBOOL checkForResult()
                {
                    if (result != NULL)
                        return (true);

                    return (false);
                }

                /// Get the result from the async function call. Be sure to set deleteOnComplete = true;
                JSValueRef getResult(JSCJavascriptEngine *engine);

                JSObjectRef func;
                JSObjectRef args;
                RJBOOL deleteOnComplete;

                JSValueRef result;
        };
        
		typedef std::unordered_map<unsigned int,
					std::pair<JSObjectRef,
						std::chrono::time_point<std::chrono::steady_clock> > > TimerMap;

		namespace CPP
		{
			class ContextManager;
		}

		class WorkNotificator;
		
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
                void executeScript(String code, String fileName, JSObjectRef context);
				/// Execute Javascript code.
                void executeScript(String code, String fileName);
                /// Execute Javascript code.
                void executeScript(JSStringRef code, JSStringRef fileName, JSObjectRef context = NULL);
                /// Execute javascript on the next tick.
                void executeScriptNextTick(String code, String fileName, JSObjectRef context = NULL);
                /// Call a function on the next tick. Any args passed MUST be an array.
                void callFunctionOnNextTick(AsyncFunctionCall *call);
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
				RJUINT addTimeout(JSObjectRef func, RJINT time);
				/// Clear timeout
				void clearTimeout(RJUINT timerId);

				/// A blockchain event has occurred, process it.
				void blockchainEvent(String event, String dataType = "null", void *data = NULL);
				//void blockchainEvent(String event, RJINT numargs, v8::Local<v8::Value> *args, RJBOOL alreadyEnteredCritialSection = false);

				/// Add a thread to be handled by the engine.
				void addThread(Thread *thread);
				/// Remove a thread.
				void removeThread(Thread *thread);
				/// Throw a Javascript exception.
				void throwException(String message);
				/// Throw a Javascript exception.
				void throwException(JSContextRef ctx, JSValueRef* exception, String message);

				/// Shutdown the application entirely.
				void exit(RJINT exitCode);

				/// Get a native object.
				CPP::ChainedPtr* jscGetExternal(JSContextRef context, JSObjectRef handle, String functionName);
				/// Get a native object.
				template<class T>
				std::shared_ptr<T> jscGetExternal(JSContextRef context, JSObjectRef handle, String functionName)
				{
					return externalsManager->get<T>(context, handle, functionName);
				}
				/// Set and wrap external object
				void jscSetExternal(JSContextRef context, JSObjectRef handle, String functionName, CPP::ChainedPtr *obj);
				/// Set and wrap external object
				template<class T>
				void jscSetExternal(JSContextRef context, JSObjectRef handle, String functionName, std::shared_ptr<T> obj)
				{
					externalsManager->set<T>(context, handle, functionName, obj);
				}
				/// Clear external field
				void jscClearExternal(JSContextRef context, JSObjectRef handle, String functionName);

                /// Create an exception.
                JSObjectRef jscCreateExternal (void *external, std::function<void (JSObjectRef jsObject)> onDelete);
                /// Create an exception.
                JSObjectRef jscCreateExternal (JSContextRef context, void *external, std::function<void (JSObjectRef jsObject)> onDelete);
                /// Create an exception.
                JSValueRef jscCreateException ();
                /// Create an exception.
                JSValueRef jscCreateException (JSContextRef context);
                /// If necessary, handle an exception.
                RJBOOL jscHandleException (JSValueRef exception);
                /// If necessary, handle an exception.
                RJBOOL jscHandleException (JSContextRef context, JSValueRef exception);

                /// Cast a value to an object.
                JSObjectRef jscCastValueToObject (JSValueRef value);
                /// Cast a value to an object.
                JSObjectRef jscCastValueToObject (JSContextRef context, JSValueRef value);
            
                /// Cast a value to a RJINT.
                inline RJINT jscValueToInt (JSValueRef value)
                {
                    return (jscValueToNumber (globalContext, value));
                }

                /// Cast a value to a RJINT.
                inline RJINT jscValueToInt (JSContextRef context, JSValueRef value)
                {
                    return (jscValueToNumber (context, value));
                }

                /// Create an array.
                JSObjectRef jscCreateArray (RJINT numArgs, JSValueRef args[]);
                /// Create an array.
                JSObjectRef jscCreateArray (JSContextRef context, RJINT numArgs, JSValueRef args[]);
                /// Create a JSValueRef array from a JS array. Delete the JSValueRef array created with this method.
                JSValueRef *jscCreateArray (JSObjectRef args, RJINT *numArgs);
                /// Create a JSValueRef array from a JS array.
                JSValueRef *jscCreateArray (JSContextRef context, JSObjectRef args, RJINT *numArgs);
                /// Cast a value to a RJNUMBER.
                RJINT jscValueToNumber (JSValueRef value);
                /// Cast a value to a RJNUMBER.
                RJINT jscValueToNumber (JSContextRef context, JSValueRef value);

                /// Checks if a JSC value is undefined or null. Returns true if it is.
                RJBOOL jscIsNull(JSValueRef val);
                /// Checks if a JSC value is undefined or null. Returns true if it is.
                RJBOOL jscIsNull(JSContextRef context, JSValueRef val);
                /// Cast a value to a RJBOOL.
                RJBOOL jscValueToBoolean (JSValueRef value);
                /// Cast a value to a RJBOOL.
                RJBOOL jscValueToBoolean (JSContextRef context, JSValueRef value);
            
                /// Cast a value to a JSStringRef.
                JSStringRef jscValueToJSStringRef (JSValueRef value);
                /// Cast a value to a RJBOOL.
                JSStringRef jscValueToJSStringRef (JSContextRef context, JSValueRef value);

                /// Get a JSC function.
                JSObjectRef jscGetFunction (JSObjectRef context, String functionName);
                /// Get a JSC value.
                JSValueRef jscGetValue (JSObjectRef context, String functionName);
                /// Set a string.
                void jscSetString(JSObjectRef context, String functionName, String str);
                /// Get a string from a V8 variable name. If the value is null or has an empty handle, an empty string will return.
                String jscGetString(JSObjectRef context, String functionName);
                /// Set a number.
                void jscSetNumber(JSObjectRef context, String functionName, RDECIMAL number);
                /// Get a JSC int. If the value is null or has an empty handle, 0 will be returned.
                RJINT jscGetInt(JSObjectRef context, String functionName);
                /// Get a JSC decimal. If the value is null or has an empty handle, 0 will be returned.
                RDECIMAL jscGetDecimal(JSObjectRef context, String functionName);
                /// Set a bool from a JSC boolean value.
                void jscSetBool(JSObjectRef context, String functionName, bool value);
                /// Get a JSC bool. If the value is null or has an empty handle, false will be returned.
                RJBOOL jscGetBool(JSObjectRef context, String functionName);
                /// Set a JSC object.
                void jscSetObject(JSObjectRef context, String functionName, JSObjectRef obj);
                /// Get a JSC object.
                JSObjectRef jscGetObject(JSObjectRef context, String functionName);
                /// Call a JSC function.
                JSValueRef jscCallFunction(JSObjectRef context, String functionName, RJINT numArgs, JSValueRef args[]);
                /// Call a JSC function as a constructor.
                JSObjectRef jscCallAsConstructor(JSObjectRef function, RJINT numArgs, JSValueRef args[]);
                /// Create a new object.
                JSObjectRef jscCreateNewObject(String objectName, RJINT numArgs = 0, JSValueRef args[] = NULL);
                /// Get an object, class, or namespace from a JS class.
                JSObjectRef jscGetObjectFromJSClass(String objectName, JSContextRef context = NULL);
                /// Get a bool value from a JS value.
                RJBOOL jscParseBool(JSValueRef val);
                /// Get a bool value from a JS value.
                RJBOOL jscParseBool(JSContextRef context, JSValueRef val);
				/// Get a decimal value from a JS value.
				RDECIMAL jscParseDecimal(JSValueRef val);
				/// Get a decimal value from a JS value.
				RDECIMAL jscParseDecimal(JSContextRef context, JSValueRef val);
                /// Get a int value from a JS value.
                RJINT jscParseInt(JSValueRef val);
                /// Get a int value from a JS value.
                RJINT jscParseInt(JSContextRef context, JSValueRef val);
				/// Get object properties
				std::vector<std::string> jscGetObjectPropertyNames(JSObjectRef context);

				// Create a promise.
				JSObjectRef createPromise(JSObjectRef function);
				// Create a promise.
				JSObjectRef createPromise(
                        JSObjectRef context, JSObjectRef function, RJINT numArgs = 0, JSValueRef args[] = NULL);

                /// Get an element from an array.
                JSValueRef jscGetElement(JSObjectRef array, RJUINT index);
                /// Get an element from an array.
                JSValueRef jscGetElement(JSContextRef context, JSObjectRef array, RJUINT index);

				//Pick up argument from array of callback function arguments
				JSValueRef jscGetArgument(const JSValueRef arguments[], RJULONG argumentCount, RJULONG index);
			
				//Pick up and treat argument as string from JS String, Array or ArrayBuffer in callback function
				String jscGetArgumentAsString(JSContextRef ctx, const JSValueRef arguments[], RJULONG argumentCount, RJULONG index);
			
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
				CPP::ContextManager* contextManager;

			protected:
				template<class T>
				void initJSCCallback(JSObjectRef parent,
									 const char* nameSpace,
									 const char* typeName);

			protected:
				Array<String> jsToExecuteNextCode;
                Array<String> jsToExecuteNextFilename;
                Array<JSObjectRef> jsToExecuteNextContext;

                Array<AsyncFunctionCall *> funcs;

				TimerMap timers;

                ExternalsManager* externalsManager;
				WorkNotificator* workNotificator;
				String applicationScriptFilePath;
		};
	}
#endif
#endif
