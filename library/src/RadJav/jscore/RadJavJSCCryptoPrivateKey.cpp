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
#include "jscore/RadJavJSCCryptoPrivateKey.h"
#include "jscore/RadJavJSCCryptoPublicKey.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPCryptoPrivateKey.h"
#include "cpp/RadJavCPPCryptoPublicKey.h"

#include <orb/ORB_EngineCrypto.h>
#define ENGINE CPP::Crypto::PrivateKey
#define PUB_KEY CPP::Crypto::PublicKey

#include <iostream>
#include <tuple>
#include <cstring>
#include <sstream>
#include <iomanip>

void printHex(const std::string& prefix, const void *data, int dataLength);

namespace RadJAV
{
	namespace JSC
	{
		namespace Crypto
		{
			void PrivateKey::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_init", PrivateKey::_init);
				JSC_CALLBACK(object, "sign", PrivateKey::sign);
				JSC_CALLBACK(object, "signSync", PrivateKey::signSync);
				JSC_CALLBACK(object, "encrypt", PrivateKey::encrypt);
				JSC_CALLBACK(object, "encryptSync", PrivateKey::encryptSync);
				JSC_CALLBACK(object, "getPublicKey", PrivateKey::getPublicKey);
				JSC_CALLBACK(object, "savePemSync", PrivateKey::savePemSync);
			}
			
			JSValueRef PrivateKey::_init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<ENGINE> engine(RJNEW ENGINE(JSC_JAVASCRIPT_ENGINE, ctx, argumentCount, arguments), [](ENGINE* p){DELETEOBJ(p)});
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_engine", engine);
				
				JSC_JAVASCRIPT_ENGINE->jscSetString(thisObject, "algorithm",
													engine -> myAlgorithm);
				JSC_JAVASCRIPT_ENGINE->jscSetString(thisObject, "cryptoLibrary",
													engine -> myCryptoLibrary);
				JSC_JAVASCRIPT_ENGINE->jscSetString(thisObject, "inputEncoding",
													engine -> myInputEncoding);
				JSC_JAVASCRIPT_ENGINE->jscSetString(thisObject, "outputEncoding",
													engine -> myOutputEncoding);
				
				return JSValueMakeUndefined(ctx);
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
			 
			 
			 void PrivateKey::makeInstance(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			 {
			 std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;
			 v8::Isolate* isolate = args.GetIsolate();
			 
			 std::shared_ptr<ENGINE> engine(RJNEW ENGINE(JSC_JAVASCRIPT_ENGINE, args), [](ENGINE* p){DELETEOBJ(p)});
			 JSC_JAVASCRIPT_ENGINE->jscSetExternal(thisObject, "_engine", engine);
			 
			 if (args.IsConstructCall()) {
			 // Invoked as constructor: `new MyObject(...)`
			 
			 
			 //MyObject* obj = new MyObject(value);
			 //obj->Wrap(thisObject);
			 return JSValueMake(thisObject);
			 } else {
			 // Invoked as plain function `MyObject(...)`, turn into construct call.
			 const int argc = 1;
			 v8::Local<v8::Value> argv[argc] = { args[0] };
			 v8::Local<v8::Function> cons = v8::Local<v8::Function>::New(isolate, constructor);
			 v8::Local<v8::Context> context = isolate->GetCurrentContext();
			 v8::Local<v8::Object> instance =
			 cons->NewInstance(context, argc, argv).ToLocalChecked();
			 return JSValueMake(instance);
			 }
			 }
			 */
			
			JSObjectRef PrivateKey::newInstance(JSContextRef ctx,
												JSObjectRef privateKeyParams)
			{
				JSObjectRef cryptoJs = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "Crypto");
				JSObjectRef privateKeyJs = JSC_JAVASCRIPT_ENGINE->jscGetFunction(cryptoJs, "PrivateKey");
				
				JSValueRef args[1] = { privateKeyParams };
				
				JSObjectRef instance = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(privateKeyJs, 1, args);
				
				return instance;
			}
			
			JSValueRef PrivateKey::signSync(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				std::shared_ptr<ENGINE> engine = JSC_JAVASCRIPT_ENGINE->jscGetExternal<ENGINE>(ctx, thisObject, "_engine");
				
				if (!engine)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("PrivateKey not initialized");
					return undefined;
				}
				
				if (!argumentCount)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("No arguments supplied!");
					return undefined;
				}
				
				JSObjectRef ret;
				
				String data = JSC_JAVASCRIPT_ENGINE->jscGetArgumentAsString(ctx, arguments, argumentCount, 0);
				
				if (data.empty())
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Only ArrayBuffers and Strings are supported");
					return undefined;
				}
				
				try
				{
					engine -> sign(data.c_str(), data.size(),
								   [&ret, ctx](const std::string &str)
								   {
									   ret = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, String(str).toJSCValue(ctx));
								   },
								   [&ret, ctx](void* bufPtr, int bufLen)
								   {
									   auto buffer = RJNEW unsigned char[bufLen];
									   memcpy(buffer, bufPtr, bufLen);
									   auto bufferDeallocator = [](void* bytes, void* deallocatorContext)
									   {
										   auto data = static_cast<unsigned char*>(bytes);
										   if (data) DELETEARRAY(data);
									   };
									   
									   ret = JSObjectMakeArrayBufferWithBytesNoCopy(ctx, buffer, bufLen, bufferDeallocator, nullptr, nullptr);
								   }
								   );
				}
				catch (std::invalid_argument &e)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(e.what());
				}
				
				
				return ret;
			} // End of signSync()
			
			JSValueRef PrivateKey::sign(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				std::shared_ptr<ENGINE> engine = JSC_JAVASCRIPT_ENGINE->jscGetExternal<ENGINE>(ctx, thisObject, "_engine");
				
				if (!engine)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("PrivateKey not initialized");
					return undefined;
				}
				
				if (!argumentCount)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("No arguments supplied!");
					return undefined;
				}
				
				String data = JSC_JAVASCRIPT_ENGINE->jscGetArgumentAsString(ctx, arguments, argumentCount, 0);
				
				if (data.empty())
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Only ArrayBuffers and Strings are supported");
					return undefined;
				}
				
				PromiseThread *thread = RJNEW PromiseThread();
				
				JSObjectRef promise = thread->createJSCPromise(JSC_JAVASCRIPT_ENGINE, thisObject);
				thread->onStart = [thread, data, engine, ctx]()
				{
					JSObjectRef args2 = nullptr;
					
					try
					{
						engine -> sign(data.c_str(), data.size(),
									   [&args2, ctx](const std::string &str)
									   {
										   JSValueRef strVal = String(str).toJSCValue(ctx);
										   args2 = JSObjectMakeArray(ctx, 1, &strVal, nullptr);
									   },
									   [&args2, ctx](void* bufPtr, int bufLen)
									   {
										   auto buffer = RJNEW unsigned char[bufLen];
										   memcpy(buffer, bufPtr, bufLen);
										   auto bufferDeallocator = [](void* bytes, void* deallocatorContext)
										   {
											   auto data = static_cast<unsigned char*>(bytes);
											   if (data) DELETEARRAY(data);
										   };
										   
										   JSObjectRef arrayBufferObject = JSObjectMakeArrayBufferWithBytesNoCopy(ctx, buffer, bufLen, bufferDeallocator, nullptr, nullptr);
										   args2 = JSObjectMakeArray(ctx, 1, &arrayBufferObject, nullptr);
									   }
									   );
					}
					catch (std::invalid_argument &e)
					{
						JSC_JAVASCRIPT_ENGINE->throwException(e.what());
					}
					
					thread->setResolveArgs(args2);
					
					thread->resolvePromise();
				};
				thread->onComplete = [thread]()
				{
					JSC_JAVASCRIPT_ENGINE->removeThread(thread);
				};
				
				JSC_JAVASCRIPT_ENGINE->addThread(thread);
				
				return promise;
			} // End of sign()
			
			
			JSValueRef PrivateKey::encryptSync(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				std::shared_ptr<ENGINE> engine = JSC_JAVASCRIPT_ENGINE->jscGetExternal<ENGINE>(ctx, thisObject, "_engine");
				
				if (!engine)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("PrivateKey not initialized");
					return undefined;
				}
				
				if (!argumentCount)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("No arguments supplied!");
					return undefined;
				}
				
				JSObjectRef ret;
				
				String data = JSC_JAVASCRIPT_ENGINE->jscGetArgumentAsString(ctx, arguments, argumentCount, 0);
				
				if (data.empty())
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Only ArrayBuffers and Strings are supported");
					return undefined;
				}
				
				try
				{
					engine -> encrypt(data.c_str(), data.size(),
									  [&ret, ctx](const std::string &str)
									  {
										  ret = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(String(str).toJSCValue(ctx));
									  },
									  [&ret, ctx](void* bufPtr, int bufLen)
									  {
										  auto buffer = RJNEW unsigned char[bufLen];
										  memcpy(buffer, bufPtr, bufLen);
										  auto bufferDeallocator = [](void* bytes, void* deallocatorContext)
										  {
											  auto data = static_cast<unsigned char*>(bytes);
											  if (data) DELETEARRAY(data);
										  };
										  
										  ret = JSObjectMakeArrayBufferWithBytesNoCopy(ctx, buffer, bufLen, bufferDeallocator, nullptr, nullptr);
									  }
									  );
				}
				catch (std::invalid_argument &e)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(e.what());
					return undefined;
				}
				
				return ret;
			} // End of encryptSync()
			
			JSValueRef PrivateKey::encrypt(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				std::shared_ptr<ENGINE> engine = JSC_JAVASCRIPT_ENGINE->jscGetExternal<ENGINE>(ctx, thisObject, "_engine");
				
				if (!engine)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("PrivateKey not initialized");
					return undefined;
				}
				
				if (!argumentCount)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("No arguments supplied!");
					return undefined;
				}
				
				String data = JSC_JAVASCRIPT_ENGINE->jscGetArgumentAsString(ctx, arguments, argumentCount, 0);
				
				if (data.empty())
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Only ArrayBuffers and Strings are supported");
					return undefined;
				}
				
				PromiseThread *thread = RJNEW PromiseThread();
				
				JSObjectRef promise = thread->createJSCPromise(JSC_JAVASCRIPT_ENGINE, thisObject);
				thread->onStart = [thread, data, engine, ctx]()
				{
					JSObjectRef args2 = nullptr;
					
					try
					{
						engine -> encrypt(data.c_str(), data.size(),
										  [&args2, ctx](const std::string &str)
										  {
											  JSValueRef strVal = String(str).toJSCValue(ctx);
											  args2 = JSObjectMakeArray(ctx, 1, &strVal, nullptr);
										  },
										  [&args2, ctx](void* bufPtr, int bufLen)
										  {
											  auto buffer = RJNEW unsigned char[bufLen];
											  memcpy(buffer, bufPtr, bufLen);
											  auto bufferDeallocator = [](void* bytes, void* deallocatorContext)
											  {
												  auto data = static_cast<unsigned char*>(bytes);
												  if (data) DELETEARRAY(data);
											  };
											  
											  JSObjectRef arrayBufferObject = JSObjectMakeArrayBufferWithBytesNoCopy(ctx, buffer, bufLen, bufferDeallocator, nullptr, nullptr);
											  args2 = JSObjectMakeArray(ctx, 1, &arrayBufferObject, nullptr);
										  }
										  );
					}
					catch (std::invalid_argument &e)
					{
						JSC_JAVASCRIPT_ENGINE->throwException(e.what());
					}
					
					thread->setResolveArgs(args2);
					
					thread->resolvePromise();
				};
				thread->onComplete = [thread]()
				{
					JSC_JAVASCRIPT_ENGINE->removeThread(thread);
				};
				
				JSC_JAVASCRIPT_ENGINE->addThread(thread);
				
				return promise;
			} // End of encrypt()
			
			
			JSValueRef PrivateKey::getPublicKey(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<ENGINE> engine = JSC_JAVASCRIPT_ENGINE->jscGetExternal<ENGINE>(ctx, thisObject, "_engine");
				
				if (!engine)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("PrivateKey not initialized");
					return JSValueMakeUndefined(ctx);
				}
				
				JSObjectRef publicKeyParams = JSObjectMake(ctx, nullptr, nullptr);
				
				JSC_JAVASCRIPT_ENGINE->jscSetString(publicKeyParams, "cryptoLibrary", engine->myCryptoLibrary);
				JSC_JAVASCRIPT_ENGINE->jscSetString(publicKeyParams, "algorithm", engine->myAlgorithm);
				JSC_JAVASCRIPT_ENGINE->jscSetString(publicKeyParams, "bits", engine->myBits);
				JSC_JAVASCRIPT_ENGINE->jscSetString(publicKeyParams, "encryptPadding", engine->myEncryptPadding);
				JSC_JAVASCRIPT_ENGINE->jscSetString(publicKeyParams, "signatureType", engine->mySignatureType);
				JSC_JAVASCRIPT_ENGINE->jscSetString(publicKeyParams, "inputEncoding", engine->myInputEncoding);
				JSC_JAVASCRIPT_ENGINE->jscSetString(publicKeyParams, "outputEncoding", engine->myOutputEncoding);
				
				JSObjectRef publicKeyJs = PublicKey::newInstance(ctx, publicKeyParams);
				
				// Extract C++ wrapper object from Js object, generate key and embed it in the C++ wrapper.
				std::shared_ptr<PUB_KEY> publicKeyWrap = JSC_JAVASCRIPT_ENGINE->jscGetExternal<PUB_KEY>(ctx, publicKeyJs, "_engine");
				auto publicKey = engine -> getPublicKey();
				publicKeyWrap -> setEngine(publicKey);
				
				return publicKeyJs;
			} // End of getPublicKey()
			
			JSValueRef PrivateKey::savePemSync(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				std::shared_ptr<ENGINE> engine = JSC_JAVASCRIPT_ENGINE->jscGetExternal<ENGINE>(ctx, thisObject, "_engine");
				
				if (!engine)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("PrivateKey not initialized");
					return undefined;
				}
				
				if (!argumentCount)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Path argument required");
					return undefined;
				}
				
				String path; // If a string is passed, it will be parsed and held here.
				if (JSValueIsString(ctx, arguments[0]))
					path = parseJSCValue(ctx, arguments[0]);
				else
				{
					JSC_JAVASCRIPT_ENGINE->throwException("savePemSync() requires path given as a string");
					return undefined;
				}
				
				JSValueRef cipherTypeJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);
				JSValueRef pwdJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 2);
				
				String cipherType;
				if (cipherTypeJs && JSValueIsString(ctx, cipherTypeJs))
				{
					cipherType = parseJSCValue(ctx, cipherTypeJs);
				}
				
				String pwd;
				if ( pwdJs && JSValueIsString(ctx, pwdJs))
				{
					pwd = parseJSCValue(ctx, pwdJs);
				}
				
				try
				{
					engine -> savePem(path.c_str(), cipherType.c_str(), pwd.c_str());
				}
				catch (std::invalid_argument e)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(e.what());
				}
				
				return JSValueMakeUndefined(ctx);
			} // End of savePemSync
		}
	}
}
