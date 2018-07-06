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
#include "v8/RadJavV8CryptoKeyGenerator.h"
#include "v8/RadJavV8CryptoPrivateKey.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPCryptoKeyGenerator.h"
#include "cpp/RadJavCPPCryptoPrivateKey.h"

#ifdef USE_CRYPTOGRAPHY
        #include <orb/ORB_EngineCrypto.h>
	#define ENGINE CPP::Crypto::KeyGenerator
	#define PRIV_KEY CPP::Crypto::PrivateKey
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
				void KeyGenerator::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
				{
				  //std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;

					V8_CALLBACK(object, "_init", KeyGenerator::_init);
					V8_CALLBACK(object, "generate", KeyGenerator::generate);
					//std::cout << __PRETTY_FUNCTION__ << ": end" << std::endl << std::flush;
				}

				void KeyGenerator::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
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
					
					//std::cout << __PRETTY_FUNCTION__ << std::endl;
				}


				void KeyGenerator::generate(const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					std::shared_ptr<ENGINE> engine = V8_JAVASCRIPT_ENGINE->v8GetExternal<ENGINE>(args.This(), "_engine");
					v8::Isolate *isolate = args.GetIsolate();

					v8::Local<v8::Object> privateKeyParms = v8::Object::New(isolate);

					privateKeyParms -> Set(
							       v8::String::NewFromUtf8(isolate, "cryptoLibrary"),
							       v8::String::NewFromUtf8(isolate, engine -> myCryptoLibrary.c_str())
							       );
					privateKeyParms -> Set(
							       v8::String::NewFromUtf8(isolate, "algorithm"),
							       v8::String::NewFromUtf8(isolate, engine -> myAlgorithm.c_str())
							       );
					privateKeyParms -> Set(
							       v8::String::NewFromUtf8(isolate, "bits"),
							       v8::String::NewFromUtf8(isolate, engine -> myBits.c_str())
							       );
					privateKeyParms -> Set(
							       v8::String::NewFromUtf8(isolate, "encryptPadding"),
							       v8::String::NewFromUtf8(isolate, engine -> myEncryptPadding.c_str())
							       );
					privateKeyParms -> Set(
							       v8::String::NewFromUtf8(isolate, "signatureType"),
							       v8::String::NewFromUtf8(isolate, engine -> mySignatureType.c_str())
							       );
					privateKeyParms -> Set(
							       v8::String::NewFromUtf8(isolate, "inputEncoding"),
							       v8::String::NewFromUtf8(isolate, engine -> myInputEncoding.c_str())
							       );
					privateKeyParms -> Set(
							       v8::String::NewFromUtf8(isolate, "outputEncoding"),
							       v8::String::NewFromUtf8(isolate, engine -> myOutputEncoding.c_str())
							       );

					v8::Local<v8::Object> privateKeyJs = PrivateKey::newInstance(isolate, privateKeyParms);
					
					// Extract C++ wrapper object from Js object, generate key and embed it in the C++ wrapper.
					std::shared_ptr<PRIV_KEY> privateKeyWrap = V8_JAVASCRIPT_ENGINE->v8GetExternal<PRIV_KEY>(privateKeyJs, "_engine");
					auto privateKey = engine -> generate();
					privateKeyWrap -> setEngine(privateKey);
					args.GetReturnValue().Set(privateKeyJs);

				} // End of generate()
		    
		  
		  

				  

			#endif
		}
	}
}
#endif

