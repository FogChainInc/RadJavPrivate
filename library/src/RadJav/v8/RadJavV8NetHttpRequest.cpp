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
#include "v8/RadJavV8NetHttpRequest.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPNetHttpRequest.h"

//#ifdef USE_CRYPTOGRAPHY
#define ENGINE CPP::Net::HttpRequest2
//#endif

namespace RadJAV
{
	namespace V8B
	{
		namespace Net
		{
			void HttpRequest::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "_init", HttpRequest::_init);
				V8_CALLBACK(object, "newRequest", HttpRequest::newRequest);
				V8_CALLBACK(object, "connect", HttpRequest::connect);
				V8_CALLBACK(object, "fetch", HttpRequest::fetch);
				V8_CALLBACK(object, "fetchSync", HttpRequest::fetchSync);
				
			}

			void HttpRequest::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
			{


				 v8::Isolate *isolate = args.GetIsolate();
				 std::map<std::string, std::string> parms;

				 if (args.Length() == 0)
				   {
				   }
				 else if (args.Length() == 1 && args[0] -> IsObject())
				   {
					
				     v8::Local<v8::Object> parmsObj = v8::Local<v8::Object>::Cast(args[0]);


				     parms["version"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "version");
				     parms["userAgent"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "userAgent");
				  
				     std::string action = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "action");
				  
				     parms["action"] = action;

				     parms["trustStore"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "trustStore");				    
				     parms["trustStoreType"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "trustStoreType");
				     parms["trustkeyStorePwd"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "trustStorePwd");

				     parms["verifyMode"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "verifyMode");				    

				     parms["useDefaultCertificates"] = V8_JAVASCRIPT_ENGINE -> v8GetString(parmsObj, "useDefaultCertificates");

				   }
				 else
				   {
				     String msg = "Unsupported constructor";
				     isolate -> ThrowException(v8::Exception::TypeError
							       (v8::String::NewFromUtf8(isolate,
											msg.c_str())));
				   }

				 try {

				   std::shared_ptr<ENGINE> engine(RJNEW ENGINE(parms), [](ENGINE* p){DELETEOBJ(p)});
				   V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_engine", engine);
				 }
				 catch (std::exception &e)
				   {

				     isolate -> ThrowException(v8::Exception::TypeError
							       (v8::String::NewFromUtf8(isolate,
											e.what())));
				   }
				
			  
			}


			void HttpRequest::connect(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
				v8::Isolate *isolate = args.GetIsolate();
				
				
				v8::Local<v8::String> valUrl = v8::Local<v8::String>::Cast(args[0]);

				String url = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE->isolate, valUrl);

				
				PromiseThread *thread = RJNEW PromiseThread();
				v8::Local<v8::Object> promise = thread->createV8Promise(V8_JAVASCRIPT_ENGINE, args.This());
				thread->onStart = [url, thread, engine, isolate]()

				  {
				    String value;
				    
				    try
				      {
					value = engine->connect(url);

					v8::Local<v8::String> valueStr = value.toV8String(isolate);
					v8::Local<v8::Array> args2 = v8::Array::New(isolate, 1);
					args2->Set(0, valueStr);
					thread->setResolveArgs(isolate, args2);
					thread->resolvePromise();
				      }
				    catch (std::exception &e)
				      {
					value = e.what();
					v8::Local<v8::String> valueStr = value.toV8String(isolate);
					v8::Local<v8::Array> args2 = v8::Array::New(isolate, 1);
					args2->Set(0, valueStr);
					thread->setRejectArgs(isolate, args2);
					thread -> rejectPromise();
				      }

				  };
				thread->onComplete = [thread, isolate]()
				  {
				    V8_JAVASCRIPT_ENGINE->removeThread(thread);
				  };

				V8_JAVASCRIPT_ENGINE->addThread(thread);

				args.GetReturnValue().Set(promise);

			} // End of connect()
		  
			void HttpRequest::fetch(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
				v8::Isolate *isolate = args.GetIsolate();
				
				
				v8::Local<v8::String> valUrl = v8::Local<v8::String>::Cast(args[0]);

				String target = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE->isolate, valUrl);

				
				PromiseThread *thread = RJNEW PromiseThread();
				v8::Local<v8::Object> promise = thread->createV8Promise(V8_JAVASCRIPT_ENGINE, args.This());
				thread->onStart = [target, thread, engine, isolate]()

				  {
				    String value;

				    try
				      {
					value = engine->fetch(target);
					
					v8::Local<v8::String> valueStr = value.toV8String(isolate);
					v8::Local<v8::Array> args2 = v8::Array::New(isolate, 1);
					args2->Set(0, valueStr);
					thread->setResolveArgs(isolate, args2);
					thread->resolvePromise();
				      }
				    catch (std::exception &e)
				      {
					value = e.what();
					v8::Local<v8::String> valueStr = value.toV8String(isolate);
					v8::Local<v8::Array> args2 = v8::Array::New(isolate, 1);
					args2->Set(0, valueStr);
					thread->setRejectArgs(isolate, args2);
					thread -> rejectPromise();
				      }


				  };
				thread->onComplete = [thread]()
				  {
				    V8_JAVASCRIPT_ENGINE->removeThread(thread);
				  };

				V8_JAVASCRIPT_ENGINE->addThread(thread);

				args.GetReturnValue().Set(promise);

			} // End of fetch()

			void HttpRequest::fetchSync(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
				v8::Isolate *isolate = args.GetIsolate();
				
				
				v8::Local<v8::String> valUrl = v8::Local<v8::String>::Cast(args[0]);

				String target = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE->isolate, valUrl);

				String value;

				try
				  {
				    value = engine->fetch(target);
				  }
				catch (std::exception &e)
				  {
				    isolate -> ThrowException(v8::Exception::TypeError
							      (v8::String::NewFromUtf8(isolate, e.what())));
				  }
				
				v8::Local<v8::Value> ret = v8::String::NewFromUtf8(isolate,
										   value.c_str());

				args.GetReturnValue().Set(ret);

			} // End of fetchSync()
		  
			void HttpRequest::newRequest(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
				v8::Isolate *isolate = args.GetIsolate();
				
				
				v8::Local<v8::String> valUrl = v8::Local<v8::String>::Cast(args[0]);

				String url = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE->isolate, valUrl);

				
				PromiseThread *thread = RJNEW PromiseThread();
				v8::Local<v8::Object> promise = thread->createV8Promise(V8_JAVASCRIPT_ENGINE, args.This());
				thread->onStart = [url, thread, engine, isolate]()

				  {
				    String value;

				    try
				      {
					String target = engine->connect(url);
					value = engine->fetch(target);

					v8::Local<v8::String> valueStr = value.toV8String(isolate);
					v8::Local<v8::Array> args2 = v8::Array::New(isolate, 1);
					args2->Set(0, valueStr);
					thread->setResolveArgs(isolate, args2);
					thread->resolvePromise();
					
				      }
				    catch (std::exception &e)
				      {
					value = e.what();
					v8::Local<v8::String> valueStr = value.toV8String(isolate);
					v8::Local<v8::Array> args2 = v8::Array::New(isolate, 1);
					args2->Set(0, valueStr);
					thread->setRejectArgs(isolate, args2);
					thread -> rejectPromise();
				      }
				  };

				thread->onComplete = [thread, isolate]()
				  {
				    V8_JAVASCRIPT_ENGINE->removeThread(thread);
				  };

				V8_JAVASCRIPT_ENGINE->addThread(thread);

				args.GetReturnValue().Set(promise);

			} // End of newRequest()
		  
		} // Net
	}
}
#endif

