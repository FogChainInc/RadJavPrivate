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
#include "jscore/RadJavJSCCryptoKeyGenerator.h"
#include "jscore/RadJavJSCCryptoPrivateKey.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPCryptoKeyGenerator.h"
#include "cpp/RadJavCPPCryptoPrivateKey.h"

#include <orb/ORB_EngineCrypto.h>
#define ENGINE CPP::Crypto::KeyGenerator
#define PRIV_KEY CPP::Crypto::PrivateKey

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
			void KeyGenerator::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_init", KeyGenerator::_init);
				JSC_CALLBACK(object, "generate", KeyGenerator::generate);
			}
			
			JSValueRef KeyGenerator::_init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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
			
			JSValueRef KeyGenerator::generate(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				std::shared_ptr<ENGINE> engine = JSC_JAVASCRIPT_ENGINE->jscGetExternal<ENGINE>(ctx, thisObject, "_engine");
				
				if (!engine)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "KeyGenerator not initialized");
					return undefined;
				}
				
				JSObjectRef privateKeyParams = JSObjectMake(ctx, nullptr, nullptr);
				
				JSC_JAVASCRIPT_ENGINE->jscSetString(privateKeyParams, "cryptoLibrary", engine->myCryptoLibrary);
				JSC_JAVASCRIPT_ENGINE->jscSetString(privateKeyParams, "algorithm", engine->myAlgorithm);
				JSC_JAVASCRIPT_ENGINE->jscSetString(privateKeyParams, "bits", engine->myBits);
				JSC_JAVASCRIPT_ENGINE->jscSetString(privateKeyParams, "encryptPadding", engine->myEncryptPadding);
				JSC_JAVASCRIPT_ENGINE->jscSetString(privateKeyParams, "signatureType", engine->mySignatureType);
				JSC_JAVASCRIPT_ENGINE->jscSetString(privateKeyParams, "inputEncoding", engine->myInputEncoding);
				JSC_JAVASCRIPT_ENGINE->jscSetString(privateKeyParams, "outputEncoding", engine->myOutputEncoding);
				
				JSObjectRef privateKeyJs = PrivateKey::newInstance(ctx, privateKeyParams);
				
				// Extract C++ wrapper object from Js object, generate key and embed it in the C++ wrapper.
				std::shared_ptr<PRIV_KEY> privateKeyWrap = JSC_JAVASCRIPT_ENGINE->jscGetExternal<PRIV_KEY>(ctx, privateKeyJs, "_engine");
				auto privateKey = engine -> generate();
				privateKeyWrap -> setEngine(privateKey);
				
				return privateKeyJs;
			} // End of generate()
		}
	}
}
