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

#include "cpp/RadJavCPPCryptoBase.h"

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
			Base::Base(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				//std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl;
				std::cout << "Args len: " << args.Length() << std::endl;
				
				this -> jsEngine = jsEngine;
				v8::Isolate *isolate = args.GetIsolate();
				
				// Parse common settings
				if (args[0] -> IsObject())
				{
					
					v8::Local<v8::Object> parms = v8::Local<v8::Object>::Cast(args[0]);
					
					myCryptoLibrary = jsEngine -> v8GetString(parms, "cryptoLibrary");
					myAlgorithm = jsEngine -> v8GetString(parms, "algorithm");
					myInputEncoding = jsEngine -> v8GetString(parms, "inputEncoding");
					myOutputEncoding = jsEngine -> v8GetString(parms, "outputEncoding");
				}
				
				// Defaults and error checking
				if (myCryptoLibrary == "") myCryptoLibrary = "OpenSSL"; // TODO - crude way of providing a default
				if (myInputEncoding == "") myInputEncoding = "binary"; // TODO - crude way of providing a default
				if (myAlgorithm == "")
					args.GetIsolate() -> ThrowException(v8::Exception::TypeError
														(v8::String::NewFromUtf8(isolate,
																				 "'algorithm' must be defined")));
				
			}
			
			#elif defined USE_JAVASCRIPTCORE
			
			Base::Base(JSCJavascriptEngine *jsEngine, JSContextRef ctx, RJUINT argumentCount, const JSValueRef arguments[])
			{
				this->jsEngine = jsEngine;
				
				// Parse common settings
				if(argumentCount && JSValueIsObject(ctx, arguments[0]))
				{
					JSObjectRef params = jsEngine->jscCastValueToObject(ctx, arguments[0]);
					
					myCryptoLibrary = jsEngine->jscGetString(params, "cryptoLibrary");
					myAlgorithm = jsEngine->jscGetString(params, "algorithm");
					myInputEncoding = jsEngine->jscGetString(params, "inputEncoding");
					myOutputEncoding = jsEngine->jscGetString(params, "outputEncoding");
				}
				
				// Defaults and error checking
				if (myCryptoLibrary == "") myCryptoLibrary = "OpenSSL"; // TODO - crude way of providing a default
				if (myInputEncoding == "") myInputEncoding = "binary"; // TODO - crude way of providing a default
				if (myAlgorithm == "")
				{
					jsEngine->throwException("'algorithm' must be defined");
				}
			}
			#endif
			
			Base::~Base()
			{

			}

			void Base::processInput(const void *plainText, int textLength,
									std::string &decodedText,
									const void *&binPlainText, int &binPlainTextLength)
			{
				
				String _inputEncoding = myInputEncoding;
				
				//const void *binPlainText;
				//int binPlainTextLength;
				//std::string decodedText;
				
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
			}


			void Base::processOutput(std::tuple<std::shared_ptr<void>, unsigned int> result,
									 std::function <void (const std::string& str)> stringSetter,
									 std::function <void (void* buf, int bufLen)> binSetter)
			{
				
				if (myOutputEncoding == "hex")
				{
					stringSetter(ORB::Engine::Crypto::encodeHex(std::get<0>(result).get(),
																std::get<1>(result)));
					return;
				}
				else if (myOutputEncoding == "base64")
				{
					stringSetter(ORB::Engine::Crypto::encodeBase64(std::get<0>(result).get(),
																   std::get<1>(result)));
					return;
				}
				else if (myOutputEncoding == "binary")
				{
					binSetter(std::get<0>(result).get(),
							  std::get<1>(result));
					return;
				}
				String msg = "Unsupported output encoding: " + myOutputEncoding;
				throw std::invalid_argument(msg);
				
			}
		  #endif
		}
	}
}

