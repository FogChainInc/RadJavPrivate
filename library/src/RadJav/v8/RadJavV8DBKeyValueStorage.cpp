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
#include "v8/RadJavV8DBKeyValueStorage.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPDBKeyValueStorage.h"

#ifdef USE_DATABASE
	#define DBTYPE CPP::Database::KeyValueStorage
#endif

namespace RadJAV
{
	namespace V8B
	{
		namespace Database
		{
			#ifdef USE_DATABASE
				void KeyValueStorage::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
				{
					V8_CALLBACK(object, "_init", KeyValueStorage::_init);
					V8_CALLBACK(object, "open", KeyValueStorage::open);
					V8_CALLBACK(object, "write", KeyValueStorage::write);
					V8_CALLBACK(object, "read", KeyValueStorage::read);
					V8_CALLBACK(object, "close", KeyValueStorage::close);
				}

				void KeyValueStorage::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					std::shared_ptr<DBTYPE> storage(RJNEW DBTYPE(), [](DBTYPE* p){DELETEOBJ(p)});
					V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_storage", storage);
				}

				void KeyValueStorage::open(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					std::shared_ptr<DBTYPE> storage = V8_JAVASCRIPT_ENGINE->v8GetExternal<DBTYPE>(args.This(), "_storage");
					v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
					String path = parseV8Value (val);
					storage->filePath = path;

					PromiseThread *thread = RJNEW PromiseThread();
					v8::Local<v8::Object> promise = thread->createV8Promise(V8_JAVASCRIPT_ENGINE, args.This());
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
				}

				void KeyValueStorage::write(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					std::shared_ptr<DBTYPE> storage = V8_JAVASCRIPT_ENGINE->v8GetExternal<DBTYPE>(args.This(), "_storage");
					v8::Local<v8::String> key = v8::Local<v8::String>::Cast(args[0]);
					String keyStr = parseV8Value (key);
					v8::Local<v8::String> value = v8::Local<v8::String>::Cast(args[1]);
					String valueStr = parseV8Value (value);

					PromiseThread *thread = RJNEW PromiseThread();
					v8::Local<v8::Object> promise = thread->createV8Promise(V8_JAVASCRIPT_ENGINE, args.This());
					thread->onStart = [thread, storage, keyStr, valueStr]()
					{
						storage->write(keyStr, valueStr);
						thread->resolvePromise();
					};
                    thread->onComplete = [thread]()
                    {
                        V8_JAVASCRIPT_ENGINE->removeThread(thread);
                    };
					V8_JAVASCRIPT_ENGINE->addThread(thread);

					args.GetReturnValue().Set(promise);
				}

				void KeyValueStorage::read(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					std::shared_ptr<DBTYPE> storage = V8_JAVASCRIPT_ENGINE->v8GetExternal<DBTYPE>(args.This(), "_storage");
					v8::Local<v8::String> key = v8::Local<v8::String>::Cast(args[0]);
					String keyStr = parseV8Value (key);
					v8::Isolate *isolate = args.GetIsolate();

					PromiseThread *thread = RJNEW PromiseThread();
					v8::Local<v8::Object> promise = thread->createV8Promise(V8_JAVASCRIPT_ENGINE, args.This());
					thread->onStart = [thread, storage, keyStr, isolate]()
					{
						String value = storage->read(keyStr);

						v8::Local<v8::String> valueStr = value.toV8String(isolate);
						v8::Local<v8::Array> args2 = v8::Array::New (isolate, 1);
						args2->Set(0, valueStr);
						thread->setResolveArgs(isolate, args2);

						thread->resolvePromise();
					};
                    thread->onComplete = [thread]()
                    {
                        V8_JAVASCRIPT_ENGINE->removeThread(thread);
                    };

					V8_JAVASCRIPT_ENGINE->addThread(thread);

					args.GetReturnValue().Set(promise);
				}

				void KeyValueStorage::close(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					std::shared_ptr<DBTYPE> storage = V8_JAVASCRIPT_ENGINE->v8GetExternal<DBTYPE>(args.This(), "_storage");

					storage->close();
				}
			#endif
		}
	}
}
#endif

