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
#include "v8/RadJavV8CryptoHash.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPCryptoHash.h"

#ifdef USE_CRYPTOGRAPHY
        #include <orb/ORB_EngineCrypto.h>
	#define ENGINE CPP::Crypto::Hash
#endif

#include <iostream>
#include <tuple>
#include <sstream>
#include <iomanip>

namespace RadJAV
{
	namespace V8B
	{
		namespace Crypto
		{
			#ifdef USE_CRYPTOGRAPHY
				void Hash::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
				{
				  std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;

					V8_CALLBACK(object, "_init", Hash::_init);
					V8_CALLBACK(object, "digest", Hash::digest);
					V8_CALLBACK(object, "digestP", Hash::digestP);
					V8_CALLBACK(object, "read", Hash::read);

				  std::cout << __PRETTY_FUNCTION__ << ": end" << std::endl << std::flush;
				}

				void Hash::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
				  std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;
				        ENGINE *engine = RJNEW ENGINE(V8_JAVASCRIPT_ENGINE, args);
					V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_engine", engine);

					v8::Isolate *isolate = args.GetIsolate();

					V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "hashAlgorithm",
									  engine -> myHashAlgorithm);
					V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "cryptoLibrary",
									  engine -> myCryptoLibrary);
					V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "inputEncoding",
									  engine -> myInputEncoding);
					V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "outputEncoding",
									  engine -> myOutputEncoding);
					
					std::cout << __PRETTY_FUNCTION__ << std::endl;
				}


		    
		                void Hash::digest(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					ENGINE *engine = (ENGINE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_engine");
					v8::Isolate *isolate = args.GetIsolate();
					v8::Local<v8::Value> ret;

					if (args[0] -> IsString())
					  {
					    String text = parseV8Value(args[0]);
					    try
					      {
						ret = v8::String::NewFromUtf8(isolate,
									      engine -> digestString(text).c_str());
					      }
					    catch (std::invalid_argument &e)
					      {
						isolate -> ThrowException(v8::Exception::TypeError
									  (v8::String::NewFromUtf8(isolate,
												   e.what())));
					      }
					  }
					else if (args[0] -> IsArray())
					  {
					    std::cout << __PRETTY_FUNCTION__ << ": BUFFER" << std::endl << std::flush;

					  }

					args.GetReturnValue().Set(ret);

				}
		  
		  

		                void Hash::digestP(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
				        ENGINE *engine = (ENGINE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_engine");
					v8::Isolate *isolate = args.GetIsolate();


					String text;
					if (args[0] -> IsString())
					  {
					    text = parseV8Value(args[0]);
					  }
					else if (args[0] -> IsArray())
					  {
					    std::cout << __PRETTY_FUNCTION__ << ": BUFFER" << std::endl << std::flush;

					  }


					PromiseThread *thread = RJNEW PromiseThread();

					v8::Local<v8::Object> promise = thread->createV8Promise(V8_JAVASCRIPT_ENGINE, args.This());
					thread->onStart = [thread, text, engine, isolate]()
					  {
					    v8::Local<v8::Array> args2 = v8::Array::New (isolate, 1);
					    v8::Local<v8::String> digestStr = engine -> digestString(text).toV8String(isolate);
					    args2->Set(0, digestStr);
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
				  
	
				void Hash::read(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					ENGINE *engine = (ENGINE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_engine");

					v8::Local<v8::String> key = v8::Local<v8::String>::Cast(args[0]);
					String keyStr = parseV8Value (key);
					v8::Isolate *isolate = args.GetIsolate();


					PromiseThread *thread = RJNEW PromiseThread();
					v8::Local<v8::Object> promise = thread->createV8Promise(V8_JAVASCRIPT_ENGINE, args.This());
					thread->onStart = [thread, engine, keyStr, isolate]()
					{
					  String value = keyStr + "-woohoo";

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
		  
		                void Hash::getCapabilities(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
				  std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl;
					ENGINE *engine = (ENGINE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_engine");

					auto isolate = args.GetIsolate();
					
					v8::Local<v8::Object> ret = v8::Object::New(args.GetIsolate());

					v8::Local<v8::Object> cryptoLibs = v8::Object::New(args.GetIsolate());

					ret -> Set(
						   v8::String::NewFromUtf8(isolate, "cryptoLibs"),
						   cryptoLibs
						);

					v8::Local<v8::Object> openSsl = v8::Object::New(args.GetIsolate());

					cryptoLibs -> Set(
							  v8::String::NewFromUtf8(isolate, "OpenSSL"),
							  openSsl
						);

					v8::Local<v8::Array> hashAlgorithms = v8::Array::New(args.GetIsolate());

					openSsl -> Set(
						       v8::String::NewFromUtf8(isolate, "hashAlgorithms"),
						       hashAlgorithms
						       );

					
					std::map<std::string, std::string> hashAlgorithmList
					  = ORB::Engine::Crypto::getListOfDigests();

					int i=0;
					for (const auto& digest : hashAlgorithmList)
					  {
					    std::cout << digest.first << ":\t" << digest.second << std::endl;
					    v8::Local<v8::Object> digestObj = v8::Object::New(isolate);

					    hashAlgorithms -> Set(i++, digestObj);

					    digestObj -> Set(
							     v8::String::NewFromUtf8(isolate, "name"),
							     v8::String::NewFromUtf8(isolate, digest.first.c_str())
							     );

					    digestObj -> Set(
							     v8::String::NewFromUtf8(isolate, "description"),
							     v8::String::NewFromUtf8(isolate, digest.second.c_str())
							     );
					  }
					
					//v8::Local<v8::Object> ret = v8::Local<v8::Object>::New(args.GetIsolate(), obj)
					args.GetReturnValue().Set(ret);
				}

		                void Hash::getCapabilities2(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
				  std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl;
					ENGINE *engine = (ENGINE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_engine");

					String str = engine->getCapabilities();
					auto isolate = args.GetIsolate();
					

					//v8::Local<v8::String> ret = str.toV8String(args.GetIsolate());

					v8::Local<v8::Object> ret = v8::Object::New(args.GetIsolate());

					ret -> Set(
						v8::String::NewFromUtf8(isolate, "cap1"),
						str.toV8String(isolate)
						);
					
					//v8::Local<v8::Object> ret = v8::Local<v8::Object>::New(args.GetIsolate(), obj)
					args.GetReturnValue().Set(ret);
				}
		  

		    
		  
			#endif
		}
	}
}
#endif

