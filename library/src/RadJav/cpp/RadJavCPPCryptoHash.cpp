/*
	MIT-LICENSE
	Copyright (c) 2017-2018 Higher Edge Software, LLC

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

#include "cpp/RadJavCPPCryptoHash.h"


#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>

#include "RadJavString.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"
#elif defined USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"
#endif

#include <iostream>

#ifdef USE_CRYPTOGRAPHY
#include <orb/ORB_EngineCrypto.h>
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace Crypto
		{
			#ifdef USE_CRYPTOGRAPHY
			#ifdef USE_V8
			Hash::Hash(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				//			  std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl;
				std::cout << "Args len: " << args.Length() << std::endl;
				
				this -> jsEngine = jsEngine;
				v8::Isolate *isolate = args.GetIsolate();
				
				v8::Local<v8::Value> firstArg = args[0];
				
				
				// Get constructor parms for Hash(hashAlgorithm, cryptoLibrary)
				if (args[0] -> IsString())
				{
					myHashAlgorithm = parseV8Value(args[0]);
					myCryptoLibrary = parseV8Value(args[1]);
				}
				
				// Get constructor parms Hash({hashAlgorithm: ..., cryptoLibrary: ..., ...})
				else if (args[0] -> IsObject())
				{
					
					v8::Local<v8::Object> parms = v8::Local<v8::Object>::Cast(args[0]);
					
					myHashAlgorithm = jsEngine->v8GetString(parms, "hashAlgorithm");
					if (myHashAlgorithm == "")
						args.GetIsolate() -> ThrowException(v8::Exception::TypeError
															(v8::String::NewFromUtf8(isolate,
																					 "'hashAlgorithm' must be defined")));
					myCryptoLibrary = jsEngine->v8GetString(parms, "cryptoLibrary");
					
					myInputEncoding = jsEngine->v8GetString(parms, "inputEncoding");
					myOutputEncoding = jsEngine->v8GetString(parms, "outputEncoding");
				}
				
				
				// Defaults and error checking
				if (myCryptoLibrary == "") myCryptoLibrary = "OpenSSL"; // TODO - crude way of providing a default
				if (myInputEncoding == "") myInputEncoding = "binary";
				if (myOutputEncoding == "") myOutputEncoding = "binary";
				
				if (myInputEncoding != "binary" && myInputEncoding != "hex" && myInputEncoding != "base64")
				{
					String msg = "Unsupported input encoding: " + myInputEncoding;
					isolate -> ThrowException(v8::Exception::TypeError
											  (v8::String::NewFromUtf8(isolate,
																	   msg.c_str())));
					
				}
				
				if (myOutputEncoding != "binary" && myOutputEncoding != "hex" && myOutputEncoding != "base64")
				{
					String msg = "Unsupported output encoding: " + myOutputEncoding;
					isolate -> ThrowException(v8::Exception::TypeError
											  (v8::String::NewFromUtf8(isolate,
																	   msg.c_str())));
					
				}
				
				// Create the digest engine
				try
				{
					myDigest = ORB::Engine::Crypto::createDigest(myHashAlgorithm, myCryptoLibrary);
				}
				catch (std::invalid_argument& e)
				{
					String msg = myCryptoLibrary + " doesn't support the hashAlgorithm: '" + myHashAlgorithm + "'";
					isolate -> ThrowException(v8::Exception::TypeError
											  (v8::String::NewFromUtf8(args.GetIsolate(),
																	   msg.c_str())));
				}
				
				
				
				//std::cout << __PRETTY_FUNCTION__ << ": end" << std::endl;
			}
			
			#elif defined USE_JAVASCRIPTCORE
			
			Hash::Hash(JSCJavascriptEngine *jsEngine, JSContextRef ctx, RJUINT argumentCount, const JSValueRef arguments[])
			{
				this -> jsEngine = jsEngine;

				// Get constructor parms for Hash(hashAlgorithm, cryptoLibrary)
				if (argumentCount > 1 &&
					JSValueIsString(ctx, arguments[0]) &&
					JSValueIsString(ctx, arguments[1]))
				{
					myHashAlgorithm = parseJSCValue(ctx, arguments[0]);
					myCryptoLibrary = parseJSCValue(ctx, arguments[1]);
				}
				
				// Get constructor parms Hash({hashAlgorithm: ..., cryptoLibrary: ..., ...})
				else if ( argumentCount &&
						 JSValueIsObject(ctx, arguments[0]))
				{
					
					JSObjectRef params = jsEngine->jscCastValueToObject(ctx, arguments[0]);
					
					myHashAlgorithm = jsEngine->jscGetString(params, "hashAlgorithm");
					if (myHashAlgorithm == "")
						jsEngine->throwException("'hashAlgorithm' must be defined");

					myCryptoLibrary = jsEngine->jscGetString(params, "cryptoLibrary");
					
					myInputEncoding = jsEngine->jscGetString(params, "inputEncoding");
					myOutputEncoding = jsEngine->jscGetString(params, "outputEncoding");
				}
				
				
				// Defaults and error checking
				if (myCryptoLibrary == "") myCryptoLibrary = "OpenSSL"; // TODO - crude way of providing a default
				if (myInputEncoding == "") myInputEncoding = "binary";
				if (myOutputEncoding == "") myOutputEncoding = "binary";
				
				if (myInputEncoding != "binary" && myInputEncoding != "hex" && myInputEncoding != "base64")
				{
					String msg = "Unsupported input encoding: " + myInputEncoding;
					jsEngine->throwException(msg);
				}
				
				if (myOutputEncoding != "binary" && myOutputEncoding != "hex" && myOutputEncoding != "base64")
				{
					String msg = "Unsupported output encoding: " + myOutputEncoding;
					jsEngine->throwException(msg);
				}
				
				// Create the digest engine
				try
				{
					myDigest = ORB::Engine::Crypto::createDigest(myHashAlgorithm, myCryptoLibrary);
				}
				catch (std::invalid_argument& e)
				{
					String msg = myCryptoLibrary + " doesn't support the hashAlgorithm: '" + myHashAlgorithm + "'";
					jsEngine->throwException(msg);
				}
			}
			#endif
			
			Hash::~Hash()
			{

			}

			void Hash::digest(const void* plainText, int textLength,
							  std::function <void (const std::string& str)> stringSetter,
							  std::function <void (void* buf, int bufLen)> binSetter)
			{
				//std::tuple<std::shared_ptr<void>, unsigned int> cipherResult;
				//std::cout << __PRETTY_FUNCTION__ << ": String" << std::endl << std::flush;
				
				
				String _inputEncoding = myInputEncoding;
				
				const void *binPlainText;
				int binPlainTextLength;
				std::string decodedText;
				
				if (_inputEncoding == "binary")
				{
					binPlainText = plainText;
					binPlainTextLength =  textLength;
				}
				else if (_inputEncoding == "hex")
				{
					auto binData = ORB::Engine::Crypto::decodeHex(plainText, textLength);
					decodedText.assign(static_cast<const char*>(std::get<0>(binData).get()), std::get<1>(binData));
					binPlainText = decodedText.c_str();
					binPlainTextLength = decodedText.length();
				}
				else if (_inputEncoding == "base64")
				{
					auto binData = ORB::Engine::Crypto::decodeBase64(plainText, textLength);
					decodedText.assign(static_cast<const char*>(std::get<0>(binData).get()), std::get<1>(binData));
					binPlainText = decodedText.c_str();
					binPlainTextLength = decodedText.length();
				}
				else
				{
					String msg = "Unsupported input encoding: " + _inputEncoding;
					throw std::invalid_argument(msg);
				}
				
				auto digestResult = myDigest -> digest(binPlainText, binPlainTextLength);
				
				if (myOutputEncoding == "hex")
				{
					stringSetter(ORB::Engine::Crypto::encodeHex(std::get<0>(digestResult).get(),
																std::get<1>(digestResult)));
					return;
				}
				else if (myOutputEncoding == "base64")
				{
					stringSetter(ORB::Engine::Crypto::encodeBase64(std::get<0>(digestResult).get(),
																   std::get<1>(digestResult)));
					return;
				}
				else if (myOutputEncoding == "binary")
				{
					binSetter(std::get<0>(digestResult).get(),
							  std::get<1>(digestResult));
					return;
				}
				
				String msg = "Unsupported output encoding: " + myOutputEncoding;
				throw std::invalid_argument(msg);
				
			}
			#endif
		}
	}
}

