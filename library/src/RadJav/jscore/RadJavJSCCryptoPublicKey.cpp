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
#include "jscore/RadJavJSCCryptoPublicKey.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPCryptoPublicKey.h"

#include <orb/ORB_EngineCrypto.h>
#define ENGINE CPP::Crypto::PublicKey

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
			void PublicKey::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_init", PublicKey::_init);
				JSC_CALLBACK(object, "verify", PublicKey::verify);
				JSC_CALLBACK(object, "verifySync", PublicKey::verifySync);
				JSC_CALLBACK(object, "decrypt", PublicKey::decrypt);
				JSC_CALLBACK(object, "decryptSync", PublicKey::decryptSync);
				JSC_CALLBACK(object, "savePemSync", PublicKey::savePemSync);
			}
			
			JSValueRef PublicKey::_init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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
			 void PublicKey::init2(v8::Isolate* isolate, v8::Handle<v8::Function> constr)
			 {
			 std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;
			 v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, makeInstance);
			 tpl->SetClassName(v8::String::NewFromUtf8(isolate, "PublicKey"));
			 tpl->InstanceTemplate()->SetInternalFieldCount(1);
			 
			 //constructor.Reset(isolate, tpl->GetFunction());
			 constructor.Reset(isolate, constr);
			 }
			 
			 
			 void PublicKey::makeInstance(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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
			
			JSObjectRef PublicKey::newInstance(JSContextRef ctx,
											   JSObjectRef publicKeyParms)
			{
				JSObjectRef cryptoJs = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "Crypto");
				JSObjectRef publicKeyJs = JSC_JAVASCRIPT_ENGINE->jscGetFunction(cryptoJs, "PublicKey");
				
				JSValueRef argv[1] = { publicKeyParms };
				
				JSObjectRef instance = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(publicKeyJs, 1, argv);
				
				return instance;
			}
			
			JSValueRef PublicKey::verifySync(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				std::shared_ptr<ENGINE> engine = JSC_JAVASCRIPT_ENGINE->jscGetExternal<ENGINE>(ctx, thisObject, "_engine");
				
				if (!engine)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "PublicKey not initialized");
					return undefined;
				}
				
				JSValueRef dataJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				JSValueRef signatureJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);
				
				if (!dataJs || !signatureJs)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Data and signature are required parameters");
				}
				
				String data = JSC_JAVASCRIPT_ENGINE->jscGetArgumentAsString(ctx, arguments, argumentCount, 0);
				
				if (data.empty())
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Only ArrayBuffers and Strings are supported for Data");
					return undefined;
				}
				
				String signature = JSC_JAVASCRIPT_ENGINE->jscGetArgumentAsString(ctx, arguments, argumentCount, 1);
				
				if (signature.empty())
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Only ArrayBuffers and Strings are supported for Signature");
					return undefined;
				}
				
				bool result;
				try
				{
					result = engine -> verify(data.c_str(), data.size(),
											  signature.c_str(), signature.size());
					
				}
				catch (std::invalid_argument &e)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, e.what());
				}
				
				return JSValueMakeBoolean(ctx, result);
			} // End of verifySync()
			
			JSValueRef PublicKey::verify(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				std::shared_ptr<ENGINE> engine = JSC_JAVASCRIPT_ENGINE->jscGetExternal<ENGINE>(ctx, thisObject, "_engine");
				
				if (!engine)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "PublicKey not initialized");
					return undefined;
				}
				
				JSValueRef dataJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				JSValueRef signatureJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);
				
				if (!dataJs || !signatureJs)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Data and signature are required parameters");
				}
				
				String data = JSC_JAVASCRIPT_ENGINE->jscGetArgumentAsString(ctx, arguments, argumentCount, 0);
				
				if (data.empty())
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Only ArrayBuffers and Strings are supported for Data");
					return undefined;
				}
				
				String signature = JSC_JAVASCRIPT_ENGINE->jscGetArgumentAsString(ctx, arguments, argumentCount, 1);
				
				if (signature.empty())
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Only ArrayBuffers and Strings are supported for Signature");
					return undefined;
				}
				
				PromiseThread *thread = RJNEW PromiseThread();
				
				JSObjectRef promise = thread->createJSCPromise(JSC_JAVASCRIPT_ENGINE, thisObject);
				thread->onStart = [thread, data, signature, engine, ctx]()
				{
					JSObjectRef args2 = nullptr;
					
					bool result;
					try
					{
						result = engine -> verify(data.c_str(), data.size(),
												  signature.c_str(), signature.size());
						
					}
					catch (std::invalid_argument &e)
					{
						//TODO: Re-implement this! It will not work with JavaScriptCore!
						//JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, e.what());
						JSC_JAVASCRIPT_ENGINE->throwException(e.what());
					}
					
					JSValueRef resultJs = JSValueMakeBoolean(ctx, result);
					args2 = JSObjectMakeArray(ctx, 1, &resultJs, nullptr);
					
					thread->setResolveArgs(args2);
					
					thread->resolvePromise();
				};
				thread->onComplete = [thread]()
				{
					JSC_JAVASCRIPT_ENGINE->removeThread(thread);
				};
				
				JSC_JAVASCRIPT_ENGINE->addThread(thread);
				
				return promise;
			} // End of verify()
			
			JSValueRef PublicKey::decryptSync(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				std::shared_ptr<ENGINE> engine = JSC_JAVASCRIPT_ENGINE->jscGetExternal<ENGINE>(ctx, thisObject, "_engine");
				
				if (!engine)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "PublicKey not initialized");
					return undefined;
				}
				
				if (!argumentCount)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Data argument required");
					return undefined;
				}
				
				JSObjectRef ret;
				
				String data = JSC_JAVASCRIPT_ENGINE->jscGetArgumentAsString(ctx, arguments, argumentCount, 0);
				
				if (data.empty())
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Only ArrayBuffers and Strings are supported");
					return undefined;
				}
				
				try
				{
					engine -> decrypt(data.c_str(), data.size(),
									  [&ret, ctx](const std::string &str)
									  {
										  ret = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject( String(str).toJSCValue(ctx));
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
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, e.what());
				}
				
				return ret;
			} // End of decryptSync()
			
			JSValueRef PublicKey::decrypt(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				std::shared_ptr<ENGINE> engine = JSC_JAVASCRIPT_ENGINE->jscGetExternal<ENGINE>(ctx, thisObject, "_engine");
				
				if (!engine)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "PublicKey not initialized");
					return undefined;
				}
				
				if (!argumentCount)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Data argument required");
					return undefined;
				}
				
				String data = JSC_JAVASCRIPT_ENGINE->jscGetArgumentAsString(ctx, arguments, argumentCount, 0);
				
				if (data.empty())
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Only ArrayBuffers and Strings are supported");
					return undefined;
				}
				
				PromiseThread *thread = RJNEW PromiseThread();
				
				JSObjectRef promise = thread->createJSCPromise(JSC_JAVASCRIPT_ENGINE, thisObject);
				thread->onStart = [thread, data, engine, ctx]()
				{
					JSObjectRef args2 = nullptr;
					
					try
					{
						engine -> decrypt(data.c_str(), data.size(),
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
						//TODO: Re-implement this! It will not work with JavaScriptCore!
						//JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, e.what());
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
			} // End of decrypt()
			
			JSValueRef PublicKey::savePemSync(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				std::shared_ptr<ENGINE> engine = JSC_JAVASCRIPT_ENGINE->jscGetExternal<ENGINE>(ctx, thisObject, "_engine");
				
				if (!engine)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "PublicKey not initialized");
					return undefined;
				}
				
				JSValueRef pathJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (!pathJs || !JSValueIsString(ctx, pathJs))
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Path argument required");
					return undefined;
				}
				
				String path = parseJSCValue(ctx, pathJs);
				engine -> savePem(path.c_str());
				
				return undefined;
			} // End of savePemSync
		}
	}
}
