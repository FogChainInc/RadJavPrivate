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

void printHex(const std::string& prefix, const void *data, int dataLength);

namespace RadJAV
{
	namespace V8B
	{
		namespace Crypto
		{
			#ifdef USE_CRYPTOGRAPHY
				void Hash::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
				{
				  //std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;

					V8_CALLBACK(object, "_init", Hash::_init);
					V8_CALLBACK(object, "digestSync", Hash::digestSync);
					V8_CALLBACK(object, "digest", Hash::digest);

					//std::cout << __PRETTY_FUNCTION__ << ": end" << std::endl << std::flush;
				}

				void Hash::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
				  //std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;
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
					
					//std::cout << __PRETTY_FUNCTION__ << std::endl;
				}


		                void Hash::digestSync(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
				  //std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl;

					ENGINE *engine = (ENGINE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_engine");
					v8::Isolate *isolate = args.GetIsolate();
					v8::Local<v8::Value> ret;

					String strArgHolder; // If a string is passed, it will be parsed and held here.
					const void* text;
					int textLength;
					if (args[0] -> IsString())
					  {
					    strArgHolder = parseV8Value(args[0]);
					    text = strArgHolder.c_str();
					    textLength = strArgHolder.length();
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
						text = ab -> GetContents().Data();
						textLength = ab -> ByteLength();
					      }
					  }

					try
					  {
					    engine -> digest(text, textLength,
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

				} // End of digestSync()
		    
		  
		                void Hash::digest(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
				        ENGINE *engine = (ENGINE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_engine");
					v8::Isolate *isolate = args.GetIsolate();

					std::shared_ptr<String> strArgHolder; // If a string is passed, it will be parsed and held here.
					std::shared_ptr<unsigned char> arrBufArgHolder; // If ArryayBuffer is passed, it's data will be held here
					
					const void* text;
					int textLength;

					if (args[0] -> IsString())
					  {
					    strArgHolder = std::make_shared<String>(parseV8Value(args[0]));
					    text = strArgHolder -> c_str();
					    textLength = strArgHolder -> length();
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
						textLength = ab -> ByteLength();
						
						arrBufArgHolder = std::shared_ptr<unsigned char>(new unsigned char[textLength],
												 std::default_delete<unsigned char[]>());
						std::memcpy(arrBufArgHolder.get(), ab -> GetContents().Data(), textLength);
					      }
					  }

					PromiseThread *thread = RJNEW PromiseThread();

					v8::Local<v8::Object> promise = thread->createV8Promise(V8_JAVASCRIPT_ENGINE, args.This());
					thread->onStart = [thread, text, textLength, strArgHolder, arrBufArgHolder, engine, isolate]()
					  {
					    v8::Local<v8::Array> args2 = v8::Array::New (isolate, 1);

					    try
					      {
						
						engine -> digest(text, textLength,
								 [&args2, isolate](const std::string &str)
								 {
								   args2 -> Set(0, v8::String::NewFromUtf8(isolate,
													   str.c_str()));
								 },
								 [&args2, isolate](void* bufPtr, int bufLen)
								 {
								   auto jsData = v8::ArrayBuffer::New(isolate, bufLen);
								   std::memcpy(jsData -> GetContents().Data(), bufPtr, bufLen);
								   args2 -> Set(0, jsData);
								   
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

				}  // End of digest()
		  

				  
		                void Hash::getCapabilities(const v8::FunctionCallbackInfo<v8::Value> &args)
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
					
					args.GetReturnValue().Set(ret);
				} // End of getCapabilities()

			#endif
		}
	}
}
#endif

