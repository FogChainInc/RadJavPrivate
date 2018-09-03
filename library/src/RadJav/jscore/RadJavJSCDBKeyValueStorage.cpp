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
#include "jscore/RadJavJSCDBKeyValueStorage.h"

#include "RadJav.h"

#ifdef USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPDBKeyValueStorage.h"

#ifdef USE_DATABASE
    #define DBTYPE CPP::Database::KeyValueStorage
#endif

namespace RadJAV
{
	namespace JSC
	{
        #ifdef USE_DATABASE
            void Database::createJSCCallbacks(JSContextRef context, JSObjectRef object)
            {
                JSC_CALLBACK(object, "_init", KeyValueStorage::_init);
                JSC_CALLBACK(object, "open", KeyValueStorage::open);
                JSC_CALLBACK(object, "write", KeyValueStorage::write);
                JSC_CALLBACK(object, "read", KeyValueStorage::read);
                JSC_CALLBACK(object, "close", KeyValueStorage::close);
            }

            JSValueRef Database::_init(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                std::shared_ptr<DBTYPE> storage(RJNEW DBTYPE(), [](DBTYPE* p){DELETEOBJ(p)});
                JSC_JAVASCRIPT_ENGINE->jscSetExternal(context, thisObj, "_storage", storage);

                return (JSValueMakeUndefined(context));
            }

            JSValueRef Thread::open(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                std::shared_ptr<DBTYPE> storage = V8_JAVASCRIPT_ENGINE->v8GetExternal<DBTYPE>(args.This(), "_storage");
                JSValueRef val = args[0];
                String path = parseJSCValue (val);
                storage->filePath = path;
                
                PromiseThread *thread = RJNEW PromiseThread();
                JSObjectRef promise = thread->createJSCPromise(JSC_JAVASCRIPT_ENGINE, thisObj);
                thread->onStart = [thread, storage]()
                {
                    storage->open();
                    thread->resolvePromise();
                };
                thread->onComplete = [thread]()
                {
                    V8_JAVASCRIPT_ENGINE->removeThread(thread);
                };
                V8_JAVASCRIPT_ENGINE->addThread(thread);
                
                args.GetReturnValue().Set(promise);
                
                return (JSValueMakeUndefined(context));
            }

            JSValueRef Thread::write(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                TTYPE *appObject = (TTYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");

                appObject->close();
                
                return (JSValueMakeUndefined(context));
            }

            JSValueRef Thread::read(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                String event = parseJSCValue(context, args[0]);
                JSObjectRef nfunc = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject (args[1]);
                TTYPE *appObject = (TTYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");

                if (appObject != NULL)
                    appObject->on(event, nfunc);

                return (JSValueMakeUndefined(context));
            }
        
            JSValueRef Thread::close(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                String event = parseJSCValue(context, args[0]);
                JSObjectRef nfunc = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject (args[1]);
                TTYPE *appObject = (TTYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");
                
                if (appObject != NULL)
                    appObject->on(event, nfunc);
                
                return (JSValueMakeUndefined(context));
            }
        #endif
	}
}
#endif

