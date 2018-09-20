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
#include "jscore/RadJavJSCCryptoDecipher.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPCryptoDecipher.h"

#include <orb/ORB_EngineCrypto.h>
#define ENGINE CPP::Crypto::Decipher

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
			void Decipher::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_init", Decipher::_init);
				JSC_CALLBACK(object, "decipherSync", Decipher::decipherSync);
				JSC_CALLBACK(object, "decipher", Decipher::decipher);
			}
			
			JSValueRef Decipher::_init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<ENGINE> engine(RJNEW ENGINE(JSC_JAVASCRIPT_ENGINE, ctx, argumentCount, arguments), [](ENGINE* p){DELETEOBJ(p)});
				
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_engine", engine);
				
				JSC_JAVASCRIPT_ENGINE->jscSetString(thisObject, "cipherAlgorithm",
													engine -> myCipherAlgorithm);
				JSC_JAVASCRIPT_ENGINE->jscSetString(thisObject, "cryptoLibrary",
													engine -> myCryptoLibrary);
				JSC_JAVASCRIPT_ENGINE->jscSetString(thisObject, "inputEncoding",
													engine -> myInputEncoding);
				JSC_JAVASCRIPT_ENGINE->jscSetString(thisObject, "outputEncoding",
													engine -> myOutputEncoding);
				JSC_JAVASCRIPT_ENGINE->jscSetString(thisObject, "iv",
													engine -> myIv);
				
				return JSValueMakeUndefined(ctx);
			}
			
			JSValueRef Decipher::decipherSync(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				std::shared_ptr<ENGINE> engine = JSC_JAVASCRIPT_ENGINE->jscGetExternal<ENGINE>(ctx, thisObject, "_engine");
				if (!engine)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Decipher not initialized");
					return undefined;
				}
				
				if (!argumentCount)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("No arguments supplied!");
					return undefined;
				}
				
				JSObjectRef ret;
				
				String data = JSC_JAVASCRIPT_ENGINE->jscGetArgumentAsString(ctx, arguments, argumentCount, 0);
				
				if (!data.size())
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Only ArrayBuffers and Strings are supported");
					return undefined;
				}
				
				try
				{
					engine -> decipher(data.c_str(), data.size(),
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
					JSC_JAVASCRIPT_ENGINE->throwException(e.what());
					return undefined;
				}
				
				return ret;
			} // End of decipherSync()
			
			JSValueRef Decipher::decipher(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				std::shared_ptr<ENGINE> engine = JSC_JAVASCRIPT_ENGINE->jscGetExternal<ENGINE>(ctx, thisObject, "_engine");
				if (!engine)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Decipher not initialized");
					return undefined;
				}
				
				if (!argumentCount)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("No arguments supplied!");
					return undefined;
				}
				
				String data = JSC_JAVASCRIPT_ENGINE->jscGetArgumentAsString(ctx, arguments, argumentCount, 0);
				
				if (!data.size())
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
						engine -> decipher(data.c_str(), data.size(),
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
			}  // End of decipher()
			
			JSValueRef Decipher::getCapabilities(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<ENGINE> engine = JSC_JAVASCRIPT_ENGINE->jscGetExternal<ENGINE>(ctx, thisObject, "_engine");
				
				JSObjectRef ret = JSObjectMake(ctx, nullptr, nullptr);
				JSObjectRef cryptoLibs = JSObjectMake(ctx, nullptr, nullptr);
				JSC_JAVASCRIPT_ENGINE->jscSetObject(ret, "cryptoLibs", cryptoLibs);
				
				JSObjectRef openSsl = JSObjectMake(ctx, nullptr, nullptr);
				JSC_JAVASCRIPT_ENGINE->jscSetObject(cryptoLibs, "OpenSSL", openSsl);
				
				std::map<std::string, std::string> cipherAlgorithmList
				= ORB::Engine::Crypto::getListOfCiphers();
				
				const size_t cipherAlgorithmsSize = cipherAlgorithmList.size();
				JSValueRef cipherAlgorithmsArray[cipherAlgorithmsSize];
				
				int i=0;
				for (const auto& alg : cipherAlgorithmList)
				{
					JSObjectRef algObj = JSObjectMake(ctx, nullptr, nullptr);
					
					JSC_JAVASCRIPT_ENGINE->jscSetString(algObj, "name", alg.first);
					JSC_JAVASCRIPT_ENGINE->jscSetString(algObj, "description", alg.second);
					
					cipherAlgorithmsArray[i++] = algObj;
				}
				
				JSObjectRef cipherAlgorithms = JSObjectMakeArray(ctx, cipherAlgorithmsSize, cipherAlgorithmsArray, nullptr);
				JSC_JAVASCRIPT_ENGINE->jscSetObject(openSsl, "cipherAlgorithms", cipherAlgorithms);
				
				return ret;
			}
		}
	}
}
