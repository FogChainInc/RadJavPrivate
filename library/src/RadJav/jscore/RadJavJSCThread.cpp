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
#include "jscore/RadJavJSCThread.h"

#include "RadJav.h"

#ifdef USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPThread.h"

#define TTYPE CPP::Thread

namespace RadJAV
{
	namespace JSC
	{
		void Thread::createJSCCallbacks(JSContextRef context, JSObjectRef object)
		{
			JSC_CALLBACK(object, "_init", Thread::_init);
			JSC_CALLBACK(object, "start", Thread::start);
			JSC_CALLBACK(object, "close", Thread::close);
			JSC_CALLBACK(object, "on", Thread::on);
		}

        JSValueRef Thread::_init(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
		{
			TTYPE *appObject = RJNEW TTYPE();
			JSC_JAVASCRIPT_ENGINE->jscSetExternal(context, thisObj, "_appObj", appObject);

            return (JSValueMakeUndefined(context));
		}

        JSValueRef Thread::start(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
		{
			TTYPE *appObject = (TTYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");
			appObject->start();
            
            return (JSValueMakeUndefined(context));
		}

        JSValueRef Thread::close(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
		{
			TTYPE *appObject = (TTYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");

			appObject->close();
            
            return (JSValueMakeUndefined(context));
		}

        JSValueRef Thread::on(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
		{
			String event = parseJSCValue(context, args[0]);
			JSObjectRef nfunc = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject (args[1]);
			TTYPE *appObject = (TTYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");

			if (appObject != NULL)
				appObject->on(event, nfunc);

            return (JSValueMakeUndefined(context));
		}
	}
}
#endif

