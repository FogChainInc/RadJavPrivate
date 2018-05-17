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
#include "v8/RadJavV8CryptoDecipherMultipart.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPCryptoDecipherMultipart.h"

#ifdef USE_CRYPTOGRAPHY
        #include <orb/ORB_EngineCrypto.h>
	#define ENGINE CPP::Crypto::DecipherMultipart
#endif

#include <iostream>
#include <tuple>
#include <cstring>
#include <sstream>
#include <iomanip>

void printHex(const std::string& prefix, const void *data, int dataLength);

namespace RadJAV
{
	namespace V8B
	{
		namespace Crypto
		{
			#ifdef USE_CRYPTOGRAPHY
				void DecipherMultipart::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
				{
				  //std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;

					V8_CALLBACK(object, "_init", DecipherMultipart::_init);
					V8_CALLBACK(object, "updateSync", DecipherMultipart::updateSync);
					V8_CALLBACK(object, "update", DecipherMultipart::update);
					V8_CALLBACK(object, "finalize", DecipherMultipart::finalize);
					V8_CALLBACK(object, "reset", DecipherMultipart::reset);

					//std::cout << __PRETTY_FUNCTION__ << ": end" << std::endl << std::flush;
				}

				void DecipherMultipart::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
				  //std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;
				        ENGINE *engine = RJNEW ENGINE(V8_JAVASCRIPT_ENGINE, args);
					V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_engine", engine);

					v8::Isolate *isolate = args.GetIsolate();

					V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "cipherAlgorithm",
									  engine -> myCipherAlgorithm);
					V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "cryptoLibrary",
									  engine -> myCryptoLibrary);
					V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "inputEncoding",
									  engine -> myInputEncoding);
					V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "outputEncoding",
									  engine -> myOutputEncoding);
					V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "iv",
									  engine -> myIv);
					
					//std::cout << __PRETTY_FUNCTION__ << std::endl;
				}


		    
		                void DecipherMultipart::updateSync(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					ENGINE *engine = (ENGINE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_engine");
					v8::Isolate *isolate = args.GetIsolate();
					v8::Local<v8::Value> ret;


					String strArgHolder; // If a string is passed, it will be parsed and held here.
					String inputEncoding; // TODO - need to capture inputEncoding.
					const void* cipherText;
					int cipherTextLength;
					if (args[0] -> IsString())
					  {
					    strArgHolder = parseV8Value(args[0]);
					    cipherText = strArgHolder.c_str();
					    cipherTextLength = strArgHolder.length();
					  }
					else if (args[0] -> IsArray())
					  {
					    // TODO
					  }
					else if (args[0] -> IsObject())
					  {
					    String constructor = parseV8Value(v8::Local<v8::Object>::Cast(args[0]) -> GetConstructorName());
					    if (constructor == "ArrayBuffer")
					      {
						auto ab = v8::Local<v8::ArrayBuffer>::Cast(args[0]);
						cipherText = ab -> GetContents().Data();
						cipherTextLength = ab -> ByteLength();
					      }
					  }

					try
					  {
					    engine -> update(cipherText, cipherTextLength, 
							     [&ret, isolate](const std::string &str)
							     {
							       ret = v8::String::NewFromUtf8(isolate,
											     str.c_str());
							     },
							     [&ret, isolate](void* bufPtr, int bufLen)
							     {
							       auto ab = v8::ArrayBuffer::New(isolate, bufLen);
							       std::memcpy(ab -> GetContents().Data(), bufPtr, bufLen);
							       ret = ab;
							     }
							     );
					  }
					catch (std::invalid_argument &e)
					  {
					    isolate -> ThrowException(v8::Exception::TypeError
								      (v8::String::NewFromUtf8(isolate,
											       e.what())));
					  }

					args.GetReturnValue().Set(ret);
				} // End of updateSync()
		  
		                void DecipherMultipart::update(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
				        ENGINE *engine = (ENGINE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_engine");
					v8::Isolate *isolate = args.GetIsolate();
					
					std::shared_ptr<String> strArgHolder; // If a string is passed, it will be parsed and held here.
					std::shared_ptr<unsigned char> arrBufArgHolder; // If ArryayBuffer is passed, it's data will be held here
					
					const void* cipherText;
					int cipherTextLength;

					if (args[0] -> IsString())
					  {
					    strArgHolder = std::make_shared<String>(parseV8Value(args[0]));
					    cipherText = strArgHolder -> c_str();
					    cipherTextLength = strArgHolder -> length();
					    printHex("*** StrBefore: ", cipherText, cipherTextLength);
					  }
					else if (args[0] -> IsArray())
					  {
					    // TODO
					  }
					else if (args[0] -> IsObject())
					  {
					    String constructor = parseV8Value(v8::Local<v8::Object>::Cast(args[0]) -> GetConstructorName());
					    if (constructor == "ArrayBuffer")
					      {
						auto ab = v8::Local<v8::ArrayBuffer>::Cast(args[0]);
						cipherTextLength = ab -> ByteLength();
						
						arrBufArgHolder = std::shared_ptr<unsigned char>(new unsigned char[cipherTextLength],
												 std::default_delete<unsigned char[]>());
						std::memcpy(arrBufArgHolder.get(), ab -> GetContents().Data(), cipherTextLength);
					      }
					  }

					PromiseThread *thread = RJNEW PromiseThread();

					v8::Local<v8::Object> promise = thread->createV8Promise(V8_JAVASCRIPT_ENGINE, args.This());
					thread->onStart = [thread, cipherText, cipherTextLength, strArgHolder, arrBufArgHolder, engine, isolate]()
					  {
					    v8::Local<v8::Array> args2 = v8::Array::New (isolate, 1);

					    try
					      {
						printHex("*** DataInThread: ", cipherText, cipherTextLength);
						
						engine -> update(cipherText, cipherTextLength,
								 [&args2, isolate](const std::string &str)
								 {
								   args2 -> Set(0, v8::String::NewFromUtf8(isolate,
													   str.c_str()));
								   std::cout << "\t**Setting str: " << str << std::endl;
								 },
								 [&args2, isolate](void* bufPtr, int bufLen)
								 {
								   auto jsData = v8::ArrayBuffer::New(isolate, bufLen);
								   std::memcpy(jsData -> GetContents().Data(), bufPtr, bufLen);
								   args2 -> Set(0, jsData);
								   std::cout << "\t**Setting bin" << std::endl;
								   
								 }
								 );
					      }
					    catch (std::invalid_argument &e)
					      {
						isolate -> ThrowException(v8::Exception::TypeError
									  (v8::String::NewFromUtf8(isolate,
												   e.what())));
					      }
					    
					    thread->setResolveArgs(isolate, args2);

					    thread->resolvePromise();
					  };
					thread->onComplete = [thread]()
					  {
					    V8_JAVASCRIPT_ENGINE->removeThread(thread);
					  };

					V8_JAVASCRIPT_ENGINE->addThread(thread);
					args.GetReturnValue().Set(promise);
				} // End of update()

		                void DecipherMultipart::finalize(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					ENGINE *engine = (ENGINE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_engine");
					v8::Isolate *isolate = args.GetIsolate();
					v8::Local<v8::Value> ret;

					try
					  {
					    engine -> finalize(
							       [&ret, isolate](const std::string &str)
							       {
								 ret = v8::String::NewFromUtf8(isolate,
											       str.c_str());
							       },
							       [&ret, isolate](void* bufPtr, int bufLen)
							       {
								 auto ab = v8::ArrayBuffer::New(isolate, bufLen);
								 std::memcpy(ab -> GetContents().Data(), bufPtr, bufLen);
								 ret = ab;
							       }
							       );
					  }
					catch (std::invalid_argument &e)
					  {
					    isolate -> ThrowException(v8::Exception::TypeError
								      (v8::String::NewFromUtf8(isolate,
											       e.what())));
					  }

					args.GetReturnValue().Set(ret);
				} // End of finalize()

		                void DecipherMultipart::reset(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					ENGINE *engine = (ENGINE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_engine");
					v8::Isolate *isolate = args.GetIsolate();

					try
					  {
					    engine -> reset();
					  }
					catch (std::invalid_argument &e)
					  {
					    isolate -> ThrowException(v8::Exception::TypeError
								      (v8::String::NewFromUtf8(isolate,
											       e.what())));
					  }

				} // End of reset()
				  
		                void DecipherMultipart::getCapabilities(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
				  //std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl;
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

					v8::Local<v8::Array> cipherAlgorithms = v8::Array::New(args.GetIsolate());

					openSsl -> Set(
						       v8::String::NewFromUtf8(isolate, "cipherAlgorithms"),
						       cipherAlgorithms
						       );

					std::map<std::string, std::string> cipherAlgorithmList
					  = ORB::Engine::Crypto::getListOfCiphers();

					int i=0;
					for (const auto& alg : cipherAlgorithmList)
					  {
					    std::cout << alg.first << ":\t" << alg.second << std::endl;
					    v8::Local<v8::Object> algObj = v8::Object::New(isolate);

					    cipherAlgorithms -> Set(i++, algObj);

					    algObj -> Set(
							  v8::String::NewFromUtf8(isolate, "name"),
							  v8::String::NewFromUtf8(isolate, alg.first.c_str())
							  );

					    algObj -> Set(
							  v8::String::NewFromUtf8(isolate, "description"),
							  v8::String::NewFromUtf8(isolate, alg.second.c_str())
							  );
					  }
					
					args.GetReturnValue().Set(ret);
				}

			#endif
		}
	}
}
#endif

