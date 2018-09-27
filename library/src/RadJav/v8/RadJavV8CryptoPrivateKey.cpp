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
#include "v8/RadJavV8CryptoPrivateKey.h"
#include "v8/RadJavV8CryptoPublicKey.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPCryptoPrivateKey.h"
#include "cpp/RadJavCPPCryptoPublicKey.h"

#ifdef USE_CRYPTOGRAPHY
        #include <orb/ORB_EngineCrypto.h>
	#define ENGINE CPP::Crypto::PrivateKey
	#define PUB_KEY CPP::Crypto::PublicKey
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
		                v8::Persistent<v8::Function> PrivateKey::constructor;
		  
				void PrivateKey::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
				{
					//std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;

					V8_CALLBACK(object, "_init", PrivateKey::_init);
					V8_CALLBACK(object, "sign", PrivateKey::sign);
					V8_CALLBACK(object, "signSync", PrivateKey::signSync);
					V8_CALLBACK(object, "encrypt", PrivateKey::encrypt);
					V8_CALLBACK(object, "encryptSync", PrivateKey::encryptSync);
					V8_CALLBACK(object, "getPublicKey", PrivateKey::getPublicKey);
					V8_CALLBACK(object, "savePemSync", PrivateKey::savePemSync);
					//std::cout << __PRETTY_FUNCTION__ << ": end" << std::endl << std::flush;
				}
		  
				void PrivateKey::setConstructor(v8::Isolate* isolate, v8::Handle<v8::Function> constr)
				{
				  constructor.Reset(isolate, constr);
				}
		  
				void PrivateKey::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
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

		  /*
		  void PrivateKey::init2(v8::Isolate* isolate, v8::Handle<v8::Function> constr)
				{
				  std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;
				  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, makeInstance);
				  tpl->SetClassName(v8::String::NewFromUtf8(isolate, "PrivateKey"));
				  tpl->InstanceTemplate()->SetInternalFieldCount(1);

				  //constructor.Reset(isolate, tpl->GetFunction());
				  constructor.Reset(isolate, constr);
				}

		  
				void PrivateKey::makeInstance(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
				  std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;
				  v8::Isolate* isolate = args.GetIsolate();
		   
		   		  std::shared_ptr<ENGINE> engine(RJNEW ENGINE(V8_JAVASCRIPT_ENGINE, args), [](ENGINE* p){DELETEOBJ(p)});
				  V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_engine", engine);

				  if (args.IsConstructCall()) {
				    // Invoked as constructor: `new MyObject(...)`


				    //MyObject* obj = new MyObject(value);
				    //obj->Wrap(args.This());
				    args.GetReturnValue().Set(args.This());
				  } else {
				    // Invoked as plain function `MyObject(...)`, turn into construct call.
				    const int argc = 1;
				    v8::Local<v8::Value> argv[argc] = { args[0] };
				    v8::Local<v8::Function> cons = v8::Local<v8::Function>::New(isolate, constructor);
				    v8::Local<v8::Context> context = isolate->GetCurrentContext();
				    v8::Local<v8::Object> instance =
				      cons->NewInstance(context, argc, argv).ToLocalChecked();
				    args.GetReturnValue().Set(instance);
				  }
				}
		  */		    
				    
				v8::Local<v8::Object> PrivateKey::newInstance(v8::Isolate *isolate,
															  v8::Local<v8::Object> privateKeyParms)
				{

				  //std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;
				  //std::cout << "Args length: " << args.Length() << std::endl;
				  //auto isolate = args.GetIsolate();
				  const unsigned argc = 1;
				  v8::Local<v8::Value> argv[argc] = { privateKeyParms };
				  //				  v8::Local<v8::Value> argv[argc] = { };
				  
				  v8::Local<v8::Function> cons = v8::Local<v8::Function>::New(isolate, constructor);
				  v8::Local<v8::Context> context = isolate->GetCurrentContext();
				  v8::Local<v8::Object> instance =
				    cons->NewInstance(context, argc, argv).ToLocalChecked();
				  
				  //args.GetReturnValue().Set(instance);
				  return instance;
				}

				void PrivateKey::signSync(const v8::FunctionCallbackInfo<v8::Value> &args)
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
					    engine -> sign(text, textLength,
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
					
				} // End of signSync()
		  
				void PrivateKey::sign(const v8::FunctionCallbackInfo<v8::Value> &args)
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
						engine -> sign(plainText, plainTextLength,
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
				} // End of sign()
		  
		  
				void PrivateKey::encryptSync(const v8::FunctionCallbackInfo<v8::Value> &args)
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
		  
				void PrivateKey::encrypt(const v8::FunctionCallbackInfo<v8::Value> &args)
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
		  
		  
				void PrivateKey::getPublicKey(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
					v8::Isolate *isolate = args.GetIsolate();

					v8::Local<v8::Object> publicKeyParms = v8::Object::New(isolate);

					publicKeyParms -> Set(
							      v8::String::NewFromUtf8(isolate, "cryptoLibrary"),
							      v8::String::NewFromUtf8(isolate, engine -> myCryptoLibrary.c_str())
							      );
					publicKeyParms -> Set(
							      v8::String::NewFromUtf8(isolate, "algorithm"),
							      v8::String::NewFromUtf8(isolate, engine -> myAlgorithm.c_str())
							      );
					publicKeyParms -> Set(
							      v8::String::NewFromUtf8(isolate, "bits"),
							      v8::String::NewFromUtf8(isolate, engine -> myBits.c_str())
							      );
					publicKeyParms -> Set(
							      v8::String::NewFromUtf8(isolate, "encryptPadding"),
							      v8::String::NewFromUtf8(isolate, engine -> myEncryptPadding.c_str())
							      );
					publicKeyParms -> Set(
							      v8::String::NewFromUtf8(isolate, "signatureType"),
							      v8::String::NewFromUtf8(isolate, engine -> mySignatureType.c_str())
							      );
					publicKeyParms -> Set(
							      v8::String::NewFromUtf8(isolate, "inputEncoding"),
							      v8::String::NewFromUtf8(isolate, engine -> myInputEncoding.c_str())
							      );
					publicKeyParms -> Set(
							      v8::String::NewFromUtf8(isolate, "outputEncoding"),
							      v8::String::NewFromUtf8(isolate, engine -> myOutputEncoding.c_str())
							      );

					v8::Local<v8::Object> publicKeyJs = PublicKey::newInstance(isolate, publicKeyParms);
					
					// Extract C++ wrapper object from Js object, generate key and embed it in the C++ wrapper.
				    std::shared_ptr<PUB_KEY> publicKeyWrap = V8_JAVASCRIPT_ENGINE->v8GetExternal<PUB_KEY>(publicKeyJs, "_engine");
					auto publicKey = engine -> getPublicKey();
					publicKeyWrap -> setEngine(publicKey);
					args.GetReturnValue().Set(publicKeyJs);
				} // End of getPublicKey()

				void PrivateKey::savePemSync(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
					v8::Isolate *isolate = args.GetIsolate();
					v8::Local<v8::Value> ret;

					String path; // If a string is passed, it will be parsed and held here.
					if (args[0] -> IsString())
					    path = parseV8Value(args[0]);
					else
					    isolate ->
					      ThrowException(v8::Exception::TypeError
							     (v8::String::NewFromUtf8(isolate,
										      "savePemSync() requires path given as a string")));
					String cipherType;
					const char *cipherTypePtr = nullptr;
					if (args[1] -> IsString())
					  {
					    cipherType = parseV8Value(args[1]);
					    cipherTypePtr = cipherType.c_str();
					  }
					String pwd;
					const char *pwdPtr = nullptr;
					if (args[2] -> IsString())
					  {
					    pwd = parseV8Value(args[1]);
					    pwdPtr = pwd.c_str();
					  }

					try
					  {
					    engine -> savePem(path.c_str(), cipherTypePtr, pwdPtr);
					  }
					catch (std::invalid_argument e)
					  {
					    isolate -> ThrowException(v8::Exception::TypeError
								      (v8::String::NewFromUtf8(isolate,
											       e.what())));
					  }
					
				} // End of savePemSync
		  

			#endif
		}
	}
}
#endif

