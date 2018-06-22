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
#ifndef _RADJAV_CPP_CRYPTO_BASE_H_
	#define _RADJAV_CPP_CRYPTO_BASE_H_

	#include "RadJavPreprocessor.h"

	#include <string>
	#include <memory>
	#include <functional>



	#include "RadJav.h"
	#include "RadJavString.h"
	#include "RadJavHashMap.h"
	#include "RadJavThread.h"

#ifdef USE_CRYPTOGRAPHY

#endif 

	namespace RadJAV
	{
		namespace CPP
		{
		        namespace Crypto
			{
			        class RADJAV_EXPORT Base
				{
				public:
				  Base(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
				  virtual ~Base();

				  void processInput(const void *plainText, int textLength,
						    std::string &decodedText,
						    const void *&binPlainText, int &binPlainTextLength);
				  void processOutput(std::tuple<std::shared_ptr<void>, unsigned int> result,
						     std::function <void (const std::string& str)> stringSetter,
						     std::function <void (void* buf, int bufLen)> binSetter);

				  void wrap(v8::Local<v8::Object> handle);
				  
				public:
				  String myCryptoLibrary;
				  String myAlgorithm;
				  String myInputEncoding;
				  String myOutputEncoding;

				private:
				  static void weakCallback(const v8::WeakCallbackInfo<Base>& data);

				protected:
				  V8JavascriptEngine *jsEngine;
				  v8::Persistent<v8::Object> myPersistent;

				  
				};
			}
		}
	}
#endif

