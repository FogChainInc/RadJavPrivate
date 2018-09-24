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
#include "jscore/RadJavJSCDBKeyValueStorage.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPDBKeyValueStorage.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace Database
		{
			using CppDbObject = CPP::Database::KeyValueStorage;
			
			void KeyValueStorage::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_init", KeyValueStorage::_init);
				JSC_CALLBACK(object, "open", KeyValueStorage::open);
				JSC_CALLBACK(object, "write", KeyValueStorage::write);
				JSC_CALLBACK(object, "read", KeyValueStorage::read);
				JSC_CALLBACK(object, "close", KeyValueStorage::close);
			}
			
			JSValueRef KeyValueStorage::_init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				std::shared_ptr<CppDbObject> storage(RJNEW CppDbObject(), [](CppDbObject* p){DELETEOBJ(p)});
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_storage", storage);
			}
			
			JSValueRef KeyValueStorage::open(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				std::shared_ptr<CppDbObject> storage = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppDbObject>(ctx, thisObject, "_storage");
				
				if (!storage)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "KeyValueStorage not initialized");
					return JSValueMakeUndefined(ctx);
				}

				JSValueRef pathValue = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);

				if (pathValue && JSValueIsString(ctx, pathValue))
				{
					String path = parseJSCValue(ctx, pathValue);
					storage->filePath = path;
					
					PromiseThread *thread = RJNEW PromiseThread();
					JSObjectRef promise = thread->createJSCPromise(JSC_JAVASCRIPT_ENGINE, thisObject);
					thread->onStart = [thread, storage]()
					{
						storage->open();
						thread->resolvePromise();
					};
					thread->onComplete = [thread]()
					{
						JSC_JAVASCRIPT_ENGINE->removeThread(thread);
					};
					JSC_JAVASCRIPT_ENGINE->addThread(thread);
					
					return promise;
				}
				
				JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Database path required");
				return JSValueMakeUndefined(ctx);
			}
			
			JSValueRef KeyValueStorage::write(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				std::shared_ptr<CppDbObject> storage = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppDbObject>(ctx, thisObject, "_storage");
				
				if (!storage)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "KeyValueStorage not initialized");
					return JSValueMakeUndefined(ctx);
				}

				JSValueRef keyValue = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				JSValueRef valueValue = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);

				if (keyValue && JSValueIsString(ctx, keyValue) &&
					valueValue && JSValueIsString(ctx, valueValue))
				{
					String keyStr = parseJSCValue(ctx, keyValue);
					String valueStr = parseJSCValue(ctx, valueValue);
					
					PromiseThread *thread = RJNEW PromiseThread();
					JSObjectRef promise = thread->createJSCPromise(JSC_JAVASCRIPT_ENGINE, thisObject);
					thread->onStart = [thread, storage, keyStr, valueStr]()
					{
						storage->write(keyStr, valueStr);
						thread->resolvePromise();
					};
					thread->onComplete = [thread]()
					{
						JSC_JAVASCRIPT_ENGINE->removeThread(thread);
					};
					JSC_JAVASCRIPT_ENGINE->addThread(thread);
					
					return promise;
				}
				
				JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Key and Value required");
				return JSValueMakeUndefined(ctx);
			}
			
			JSValueRef KeyValueStorage::read(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				std::shared_ptr<CppDbObject> storage = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppDbObject>(ctx, thisObject, "_storage");
				
				if (!storage)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "KeyValueStorage not initialized");
					return JSValueMakeUndefined(ctx);
				}

				JSValueRef keyValue = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (keyValue && JSValueIsString(ctx, keyValue))
				{
					String keyStr = parseJSCValue(ctx, keyValue);
					
					PromiseThread *thread = RJNEW PromiseThread();
					JSObjectRef promise = thread->createJSCPromise(JSC_JAVASCRIPT_ENGINE, thisObject);
					thread->onStart = [thread, storage, keyStr, ctx]()
					{
						String value = storage->read(keyStr);
						
						JSValueRef valueStr = value.toJSCValue(ctx);
						JSObjectRef args2 = JSObjectMakeArray(ctx, 1, &valueStr, nullptr);
						
						thread->setResolveArgs(args2);
						
						thread->resolvePromise();
					};
					thread->onComplete = [thread]()
					{
						JSC_JAVASCRIPT_ENGINE->removeThread(thread);
					};
					
					JSC_JAVASCRIPT_ENGINE->addThread(thread);
					
					return promise;
				}
				
				JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Key required");
				return JSValueMakeUndefined(ctx);
			}
			
			JSValueRef KeyValueStorage::close(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				std::shared_ptr<CppDbObject> storage = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppDbObject>(ctx, thisObject, "_storage");
				
				if (storage)
					storage->close();
			}
		}
	}
}
