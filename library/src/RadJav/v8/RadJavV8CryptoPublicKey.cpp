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
#include "v8/RadJavV8CryptoPublicKey.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPCryptoPublicKey.h"

#ifdef USE_CRYPTOGRAPHY
        #include <orb/ORB_EngineCrypto.h>
	#define ENGINE CPP::Crypto::PublicKey
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
		                v8::Persistent<v8::Function> PublicKey::constructor;
		  
				void PublicKey::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
				{
				  //std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;

					V8_CALLBACK(object, "_init", PublicKey::_init);
					V8_CALLBACK(object, "verify", PublicKey::verify);
					V8_CALLBACK(object, "verifySync", PublicKey::verifySync);
					V8_CALLBACK(object, "encrypt", PublicKey::encrypt);
					V8_CALLBACK(object, "encryptSync", PublicKey::encryptSync);
					V8_CALLBACK(object, "decrypt", PublicKey::decrypt);
					V8_CALLBACK(object, "decryptSync", PublicKey::decryptSync);
					V8_CALLBACK(object, "savePemSync", PublicKey::savePemSync);
					//std::cout << __PRETTY_FUNCTION__ << ": end" << std::endl << std::flush;
				}
		  
				void PublicKey::setConstructor(v8::Isolate* isolate, v8::Handle<v8::Function> constr)
				{
				  constructor.Reset(isolate, constr);
				}
		  
				void PublicKey::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
				  //std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;
					std::shared_ptr<ENGINE> engine(RJNEW ENGINE(V8_JAVASCRIPT_ENGINE, args), [](ENGINE* p){DELETEOBJ(p)});
					V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_engine", engine);

					v8::Isolate *isolate = args.GetIsolate();

					V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "algorithm",
									  engine -> myAlgorithm);
					V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "cryptoLibrary",
									  engine -> myCryptoLibrary);
					V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "inputEncoding",
									  engine -> myInputEncoding);
					V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "outputEncoding",
									  engine -> myOutputEncoding);

					if (args.IsConstructCall()) 
					  std::cout << "CONSTRUCT CALL" << std::endl;
					else
					  std::cout << "NNNOOOOT CONSTRUCT CALL" << std::endl;

					
					//std::cout << __PRETTY_FUNCTION__ << std::endl;
				}
				    
				v8::Local<v8::Object> PublicKey::newInstance(v8::Isolate *isolate,
															 v8::Local<v8::Object> publicKeyParms)
				{

				  //std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;
				  //std::cout << "Args length: " << args.Length() << std::endl;
				  //auto isolate = args.GetIsolate();
				  const unsigned argc = 1;
				  v8::Local<v8::Value> argv[argc] = { publicKeyParms };
				  //				  v8::Local<v8::Value> argv[argc] = { };
				  
				  v8::Local<v8::Function> cons = v8::Local<v8::Function>::New(isolate, constructor);
				  v8::Local<v8::Context> context = isolate->GetCurrentContext();
				  v8::Local<v8::Object> instance =
				    cons->NewInstance(context, argc, argv).ToLocalChecked();
				  
				  //args.GetReturnValue().Set(instance);
				  return instance;
				}

				void PublicKey::verifySync(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
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
					    if (constructor.find("Array") != std::string::npos)
					      {
						auto ab = v8::Local<v8::ArrayBuffer>::Cast(args[0]);
						text = ab -> GetContents().Data();
						textLength = ab -> ByteLength();
					      }
					    else
					      isolate -> ThrowException(v8::Exception::TypeError
									(v8::String::NewFromUtf8(isolate, "Only ArrayBuffers are supported")));
					    
					  }
					else
					  isolate -> ThrowException(v8::Exception::TypeError
								    (v8::String::NewFromUtf8(isolate, "Unsupported argument")));
					


					String strArgHolder1; // If a string is passed, it will be parsed and held here.
					const void* text1;
					int textLength1;
					if (args[1] -> IsString())
					  {
					    strArgHolder1 = parseV8Value(args[1]);
					    text1 = strArgHolder1.c_str();
					    textLength1 = strArgHolder1.length();
					  }
					else if (args[1] -> IsArray())
					  {
					    // TODO
					  }
					else if (args[1] -> IsObject())
					  {
					    String constructor = parseV8Value(v8::Local<v8::Object>::Cast(args[1]) -> GetConstructorName());
					    if (constructor.find("Array") != std::string::npos)
					      {
						auto ab = v8::Local<v8::ArrayBuffer>::Cast(args[1]);
						text1 = ab -> GetContents().Data();
						textLength1 = ab -> ByteLength();
					      }
					    else
					      isolate -> ThrowException(v8::Exception::TypeError
									(v8::String::NewFromUtf8(isolate, "Only ArrayBuffers are supported")));
					    
					  }
					else
					  isolate -> ThrowException(v8::Exception::TypeError
								    (v8::String::NewFromUtf8(isolate, "Unsupported argument")));
					

					bool result;
					try
					  {
					    result = engine -> verify(text, textLength,
								      text1, textLength1);

					  }
					catch (std::invalid_argument &e)
					  {
					    isolate -> ThrowException(v8::Exception::TypeError
								      (v8::String::NewFromUtf8(isolate,
											       e.what())));
					  }
					
					ret = v8::Boolean::New(isolate, result);
					args.GetReturnValue().Set(ret);
					
				} // End of verifySync()
		  
				void PublicKey::verify(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
					v8::Isolate *isolate = args.GetIsolate();


					// Parse message
					std::shared_ptr<String> strArgHolder; // If a string is passed, it will be parsed and held here.
					std::shared_ptr<unsigned char> arrBufArgHolder; // If ArryayBuffer is passed, it's data will be held here
					
					const void* plainText;
					int plainTextLength;

					if (args[0] -> IsString())
					  {
					    strArgHolder = std::make_shared<String>(parseV8Value(args[0]));
					    plainText = strArgHolder -> c_str();
					    plainTextLength = strArgHolder -> length();
					  }
					else if (args[0] -> IsArray())
					  {
					    // TODO
					  }
					else if (args[0] -> IsObject())
					  {
					    String constructor = parseV8Value(v8::Local<v8::Object>::Cast(args[0]) -> GetConstructorName());
					    if (constructor.find("Array") != std::string::npos)
					      {
						auto ab = v8::Local<v8::ArrayBuffer>::Cast(args[0]);
						plainTextLength = ab -> ByteLength();
						
						arrBufArgHolder = std::shared_ptr<unsigned char>(new unsigned char[plainTextLength],
												 std::default_delete<unsigned char[]>());
						std::memcpy(arrBufArgHolder.get(), ab -> GetContents().Data(), plainTextLength);
					      }
					    else
					      isolate -> ThrowException(v8::Exception::TypeError
									(v8::String::NewFromUtf8(isolate, "Only ArrayBuffers are supported")));
					    
					  }
					else
					  isolate -> ThrowException(v8::Exception::TypeError
								    (v8::String::NewFromUtf8(isolate, "Unsupported argument")));
					


					// Parse signature
					std::shared_ptr<String> strArgHolder1; // If a string is passed, it will be parsed and held here.
					std::shared_ptr<unsigned char> arrBufArgHolder1; // If ArryayBuffer is passed, it's data will be held here
					
					const void* plainText1;
					int plainTextLength1;

					if (args[1] -> IsString())
					  {
					    strArgHolder1 = std::make_shared<String>(parseV8Value(args[1]));
					    plainText1 = strArgHolder1 -> c_str();
					    plainTextLength1 = strArgHolder1 -> length();
					  }
					else if (args[1] -> IsArray())
					  {
					    // TODO
					  }
					else if (args[1] -> IsObject())
					  {
					    String constructor = parseV8Value(v8::Local<v8::Object>::Cast(args[1]) -> GetConstructorName());
					    if (constructor.find("Array") != std::string::npos)
					      {
						auto ab = v8::Local<v8::ArrayBuffer>::Cast(args[1]);
						plainTextLength1 = ab -> ByteLength();
						
						arrBufArgHolder1 = std::shared_ptr<unsigned char>(new unsigned char[plainTextLength1],
												  std::default_delete<unsigned char[]>());
						std::memcpy(arrBufArgHolder1.get(), ab -> GetContents().Data(), plainTextLength1);
					      }
					    else
					      isolate -> ThrowException(v8::Exception::TypeError
									(v8::String::NewFromUtf8(isolate, "Only ArrayBuffers are supported")));
					    
					  }
					else
					  isolate -> ThrowException(v8::Exception::TypeError
								    (v8::String::NewFromUtf8(isolate, "Unsupported argument")));
					
					
					PromiseThread *thread = RJNEW PromiseThread();

					v8::Local<v8::Object> promise = thread->createV8Promise(V8_JAVASCRIPT_ENGINE, args.This());
					thread->onStart = [thread,
							   plainText, plainTextLength, strArgHolder, arrBufArgHolder,
							   plainText1, plainTextLength1, strArgHolder1, arrBufArgHolder1,
							   engine, isolate]()
					  {
					    v8::Local<v8::Array> args2 = v8::Array::New (isolate, 1);

					    bool result;
					    try
					      {
						result = engine -> verify(plainText, plainTextLength,
									       plainText1, plainTextLength1);

					      }
					    catch (std::invalid_argument &e)
					      {
						isolate -> ThrowException(v8::Exception::TypeError
									  (v8::String::NewFromUtf8(isolate,
												   e.what())));
					      }

					    args2 -> Set(0, v8::Boolean::New(isolate, result));
					    thread->setResolveArgs(isolate, args2);

					    thread->resolvePromise();
					  };
					thread->onComplete = [thread]()
					  {
					    V8_JAVASCRIPT_ENGINE->removeThread(thread);
					  };

					V8_JAVASCRIPT_ENGINE->addThread(thread);
					args.GetReturnValue().Set(promise);
				} // End of verify()
		  
		  
				void PublicKey::encryptSync(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
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
					    if (constructor.find("Array") != std::string::npos)
					      {
						auto ab = v8::Local<v8::ArrayBuffer>::Cast(args[0]);
						text = ab -> GetContents().Data();
						textLength = ab -> ByteLength();
					      }
					    else
					      isolate -> ThrowException(v8::Exception::TypeError
									(v8::String::NewFromUtf8(isolate, "Only ArrayBuffers are supported")));
					    
					  }
					else
					  isolate -> ThrowException(v8::Exception::TypeError
								    (v8::String::NewFromUtf8(isolate, "Unsupported argument")));
					

					try
					  {
					    engine -> encrypt(text, textLength,
							      [&ret, isolate](const std::string &str)
							      {
								std::cout << "STRING SETTER LEN: " << str.length() << std::endl;
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
					
				} // End of encryptSync()
		  
				void PublicKey::encrypt(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
					v8::Isolate *isolate = args.GetIsolate();
					
					std::shared_ptr<String> strArgHolder; // If a string is passed, it will be parsed and held here.
					std::shared_ptr<unsigned char> arrBufArgHolder; // If ArryayBuffer is passed, it's data will be held here
					
					const void* plainText;
					int plainTextLength;

					if (args[0] -> IsString())
					  {
					    strArgHolder = std::make_shared<String>(parseV8Value(args[0]));
					    plainText = strArgHolder -> c_str();
					    plainTextLength = strArgHolder -> length();
					  }
					else if (args[0] -> IsArray())
					  {
					    // TODO
					  }
					else if (args[0] -> IsObject())
					  {
					    String constructor = parseV8Value(v8::Local<v8::Object>::Cast(args[0]) -> GetConstructorName());
					    if (constructor.find("Array") != std::string::npos)
					      {
						auto ab = v8::Local<v8::ArrayBuffer>::Cast(args[0]);
						plainTextLength = ab -> ByteLength();
						
						arrBufArgHolder = std::shared_ptr<unsigned char>(new unsigned char[plainTextLength],
												 std::default_delete<unsigned char[]>());
						std::memcpy(arrBufArgHolder.get(), ab -> GetContents().Data(), plainTextLength);
					      }
					    else
					      isolate -> ThrowException(v8::Exception::TypeError
									(v8::String::NewFromUtf8(isolate, "Only ArrayBuffers are supported")));
					    
					  }
					else
					  isolate -> ThrowException(v8::Exception::TypeError
								    (v8::String::NewFromUtf8(isolate, "Unsupported argument")));
					

					PromiseThread *thread = RJNEW PromiseThread();

					v8::Local<v8::Object> promise = thread->createV8Promise(V8_JAVASCRIPT_ENGINE, args.This());
					thread->onStart = [thread, plainText, plainTextLength, strArgHolder, arrBufArgHolder, engine, isolate]()
					  {
					    v8::Local<v8::Array> args2 = v8::Array::New (isolate, 1);

					    try
					      {
						engine -> encrypt(plainText, plainTextLength,
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
				} // End of encrypt()

				void PublicKey::decryptSync(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
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
					    if (constructor.find("Array") != std::string::npos)
					      {
						auto ab = v8::Local<v8::ArrayBuffer>::Cast(args[0]);
						text = ab -> GetContents().Data();
						textLength = ab -> ByteLength();
					      }
					    else
					      isolate -> ThrowException(v8::Exception::TypeError
									(v8::String::NewFromUtf8(isolate, "Only ArrayBuffers are supported")));
					    
					  }
					else
					  isolate -> ThrowException(v8::Exception::TypeError
								    (v8::String::NewFromUtf8(isolate, "Unsupported argument")));
					

					try
					  {
					    engine -> decrypt(text, textLength,
							      [&ret, isolate](const std::string &str)
							      {
								std::cout << "STRING SETTER LEN: " << str.length() << std::endl;
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
					
				} // End of decryptSync()
		  
				void PublicKey::decrypt(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
					v8::Isolate *isolate = args.GetIsolate();
					
					std::shared_ptr<String> strArgHolder; // If a string is passed, it will be parsed and held here.
					std::shared_ptr<unsigned char> arrBufArgHolder; // If ArryayBuffer is passed, it's data will be held here
					
					const void* plainText;
					int plainTextLength;

					if (args[0] -> IsString())
					  {
					    strArgHolder = std::make_shared<String>(parseV8Value(args[0]));
					    plainText = strArgHolder -> c_str();
					    plainTextLength = strArgHolder -> length();
					  }
					else if (args[0] -> IsArray())
					  {
					    // TODO
					  }
					else if (args[0] -> IsObject())
					  {
					    String constructor = parseV8Value(v8::Local<v8::Object>::Cast(args[0]) -> GetConstructorName());
					    if (constructor.find("Array") != std::string::npos)
					      {
						auto ab = v8::Local<v8::ArrayBuffer>::Cast(args[0]);
						plainTextLength = ab -> ByteLength();
						
						arrBufArgHolder = std::shared_ptr<unsigned char>(new unsigned char[plainTextLength],
												 std::default_delete<unsigned char[]>());
						std::memcpy(arrBufArgHolder.get(), ab -> GetContents().Data(), plainTextLength);
					      }
					    else
					      isolate -> ThrowException(v8::Exception::TypeError
									(v8::String::NewFromUtf8(isolate, "Only ArrayBuffers are supported")));
					    
					  }
					else
					  isolate -> ThrowException(v8::Exception::TypeError
								    (v8::String::NewFromUtf8(isolate, "Unsupported argument")));
					

					PromiseThread *thread = RJNEW PromiseThread();

					v8::Local<v8::Object> promise = thread->createV8Promise(V8_JAVASCRIPT_ENGINE, args.This());
					thread->onStart = [thread, plainText, plainTextLength, strArgHolder, arrBufArgHolder, engine, isolate]()
					  {
					    v8::Local<v8::Array> args2 = v8::Array::New (isolate, 1);

					    try
					      {
						engine -> decrypt(plainText, plainTextLength,
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
				} // End of decrypt()
		  
				void PublicKey::savePemSync(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
					v8::Isolate *isolate = args.GetIsolate();
					v8::Local<v8::Value> ret;

					String path; // If a string is passed, it will be parsed and held here.
					if (args[0] -> IsString())
					  {
					    path = parseV8Value(args[0]);
					    engine -> savePem(path.c_str());
					  }
					else 
					  {
					    // TODO, error 
					  }
				} // End of savePemSync
		  
				  

			#endif
		}
	}
}
#endif

