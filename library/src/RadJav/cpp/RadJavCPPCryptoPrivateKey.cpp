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

#include "cpp/RadJavCPPCryptoPrivateKey.h"

#include "RadJavString.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"
#elif defined USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"
#endif

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>

#include <iostream>
#include <sstream>
#include <iomanip>

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
			PrivateKey::PrivateKey(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
			: Base(jsEngine, args)
			{
				//std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl;
				v8::Isolate *isolate = args.GetIsolate();
				
				
				// Get constructor parms for PrivateKey(path, format, pwd)
				String path;
				String format;
				String pwd;
				
				v8::Local<v8::Value> pathObj; //
				
				// Get constructor parms for PrivateKey(path, format, pwd, cryptoLibrary)
				if (args[0] -> IsString())
				{
					pathObj = args[0];
					
					path = parseV8Value(args[0]);
					format = parseV8Value(args[1]);
					pwd = parseV8Value(args[2]);
					myCryptoLibrary = parseV8Value(args[3]);
				}
				
				// Get constructor parms Cipher({cipherAlgorithm: ..., cryptoLibrary: ..., ...})
				else if (args[0] -> IsObject())
				{
					v8::Local<v8::Object> parms = v8::Local<v8::Object>::Cast(args[0]);
					
					myAlgorithm = jsEngine -> v8GetString(parms, "algorithm");
					myEncryptPadding = jsEngine -> v8GetString(parms, "encryptPadding");
					mySignatureType = jsEngine -> v8GetString(parms, "signatureType");
					
					pathObj = parms -> Get(v8::String::NewFromUtf8(isolate, "path"));
					if (pathObj -> IsUndefined())
					{
						myBits = jsEngine -> v8GetString(parms, "bits");
					}
					else
					{
						path = jsEngine -> v8GetString(parms, "path");
						format = jsEngine -> v8GetString(parms, "format");
						pwd = jsEngine -> v8GetString(parms, "password");
						
					}
					
				}
				
				
				if (!(pathObj -> IsUndefined()))
				{
					// Defaults and error checking
					
					// Create the engine
					try
					{
						std::map<std::string, std::string> parms;
						parms["algorithm"] = myAlgorithm;
						parms["bits"] = myBits;
						parms["encryptPadding"] = myEncryptPadding;
						parms["signatureType"] = mySignatureType;
						parms["path"] = path;
						parms["format"] = format;
						parms["password"] = pwd;
						
						myPrivateKey = ORB::Engine::Crypto::createPrivateKey(parms, myCryptoLibrary);
						
						//std::cout << __PRETTY_FUNCTION__ << ": "
						//<< "cryptoLibrary: " << myCryptoLibrary
						//<< ", ";
						for (auto element : parms)
							std::cout << element.first << ": " << element.second << ", ";
						std::cout << std::endl;
						
					}
					catch (std::exception& e)
					{
						isolate -> ThrowException(v8::Exception::TypeError
												  (v8::String::NewFromUtf8(args.GetIsolate(),
																		   e.what())));
					}
				}
				
				//std::cout << __PRETTY_FUNCTION__ << ": end" << std::endl;
				
			}

			#elif defined USE_JAVASCRIPTCORE
			
			PrivateKey::PrivateKey(JSCJavascriptEngine *jsEngine, JSContextRef ctx, RJUINT argumentCount, const JSValueRef arguments[])
			: Base(jsEngine, ctx, argumentCount, arguments)
			{
				// Get constructor parms for PrivateKey(path, format, pwd)
				String path;
				String format;
				String pwd;
				
				// Get constructor parms for PrivateKey(path, format, pwd, cryptoLibrary)
				if (argumentCount > 3 &&
					JSValueIsString(ctx, arguments[0]) &&
					JSValueIsString(ctx, arguments[1]) &&
					JSValueIsString(ctx, arguments[2]) &&
					JSValueIsString(ctx, arguments[3]))
				{
					path = parseJSCValue(ctx, arguments[0]);
					format = parseJSCValue(ctx, arguments[1]);
					pwd = parseJSCValue(ctx, arguments[2]);
					myCryptoLibrary = parseJSCValue(ctx, arguments[3]);
				}
				
				// Get constructor parms Cipher({cipherAlgorithm: ..., cryptoLibrary: ..., ...})
				else if (argumentCount &&
						 JSValueIsObject(ctx, arguments[0]))
				{
					JSObjectRef params = jsEngine->jscCastValueToObject(ctx, arguments[0]);
					
					myAlgorithm = jsEngine->jscGetString(params, "algorithm");
					myEncryptPadding = jsEngine->jscGetString(params, "encryptPadding");
					mySignatureType = jsEngine->jscGetString(params, "signatureType");
					
					JSStringRef pathName = String("path").toJSCString();
					if (JSObjectHasProperty(ctx, params, pathName) &&
						!JSValueIsUndefined(ctx, jsEngine->jscGetValue(params, "path")))
					{
						path = jsEngine->jscGetString(params, "path");
						format = jsEngine->jscGetString(params, "format");
						pwd = jsEngine->jscGetString(params, "password");
					}
					else
					{
						myBits = jsEngine->jscGetString(params, "bits");
					}

					JSStringRelease(pathName);
				}
				
				
				if (!path.empty())
				{
					// Defaults and error checking
					
					// Create the engine
					try
					{
						std::map<std::string, std::string> parms;
						parms["algorithm"] = myAlgorithm;
						parms["bits"] = myBits;
						parms["encryptPadding"] = myEncryptPadding;
						parms["signatureType"] = mySignatureType;
						parms["path"] = path;
						parms["format"] = format;
						parms["password"] = pwd;
						
						myPrivateKey = ORB::Engine::Crypto::createPrivateKey(parms, myCryptoLibrary);
						
						//std::cout << __PRETTY_FUNCTION__ << ": "
						//<< "cryptoLibrary: " << myCryptoLibrary
						//<< ", ";
						for (auto element : parms)
							std::cout << element.first << ": " << element.second << ", ";
						std::cout << std::endl;
						
					}
					catch (std::exception& e)
					{
						jsEngine->throwException(e.what());
					}
				}
			}
			#endif
			
			PrivateKey::~PrivateKey()
			{

			}


			void PrivateKey::encrypt(const void* plainText, int textLength,
									 std::function <void (const std::string& str)> stringSetter,
									 std::function <void (void* buf, int bufLen)> binSetter)
			{
				
				String _inputEncoding = myInputEncoding;
				
				const void *binPlainText;
				int binPlainTextLength;
				std::string decodedText;
				
				processInput(plainText, textLength,
							 decodedText,
							 binPlainText, binPlainTextLength);
				
				auto result = myPrivateKey -> encrypt(static_cast<const unsigned char*>(binPlainText), binPlainTextLength);
				std::cout << "Encryption Result Length: " << std::get<1>(result) << std::endl;
				
				processOutput(result, stringSetter, binSetter);
				
				
			}

			void PrivateKey::decrypt(const void* plainText, int textLength,
									 std::function <void (const std::string& str)> stringSetter,
									 std::function <void (void* buf, int bufLen)> binSetter)
			{
				
				String _inputEncoding = myInputEncoding;
				
				const void *binPlainText;
				int binPlainTextLength;
				std::string decodedText;
				
				processInput(plainText, textLength,
							 decodedText,
							 binPlainText, binPlainTextLength);
				
				auto result = myPrivateKey -> decrypt(static_cast<const unsigned char*>(binPlainText), binPlainTextLength);
				std::cout << "Encryption Result Length: " << std::get<1>(result) << std::endl;
				
				processOutput(result, stringSetter, binSetter);
				
				
			}
		  
			void PrivateKey::sign(const void* plainText, int textLength,
								  std::function <void (const std::string& str)> stringSetter,
								  std::function <void (void* buf, int bufLen)> binSetter)
			{
				
				String _inputEncoding = myInputEncoding;
				
				const void *binPlainText;
				int binPlainTextLength;
				std::string decodedText;
				
				processInput(plainText, textLength,
							 decodedText,
							 binPlainText, binPlainTextLength);
				
				auto result = myPrivateKey -> sign(static_cast<const unsigned char*>(binPlainText), binPlainTextLength);
				std::cout << "Encryption Result Length: " << std::get<1>(result) << std::endl;
				
				processOutput(result, stringSetter, binSetter);
				
			}
			
			void PrivateKey::setEngine(std::shared_ptr<Engine::Crypto::IPrivateKey> privateKey)
			{
				std::cout << "SET PRIVATE KEY" << std::endl;
				myPrivateKey = privateKey;
			}
			
			std::shared_ptr<const Engine::Crypto::IPublicKey> PrivateKey::getPublicKey()
			{
				
				return myPrivateKey -> getPublicKey();
				
			}
			
			void PrivateKey::savePem(const char *path, const char *cipherType, const char *pwd)
			{
				myPrivateKey -> savePem(path, cipherType, pwd);
			}
			#endif
		}
	}
}

