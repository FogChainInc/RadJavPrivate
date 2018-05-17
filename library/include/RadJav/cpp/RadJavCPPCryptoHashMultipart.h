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
#ifndef _RADJAV_CPP_CRYPTO_HASHMULTIPART_H_
	#define _RADJAV_CPP_CRYPTO_HASHMULTIPART_H_

	#include "RadJavPreprocessor.h"

	#include <string>
	#include <memory>


	#include "RadJav.h"
	#include "RadJavString.h"
	#include "RadJavHashMap.h"
	#include "RadJavThread.h"

#ifdef USE_CRYPTOGRAPHY

#include <i/Engine/Crypto/IDigest.h>

#endif 

	namespace RadJAV
	{
		namespace CPP
		{
			namespace Crypto
			{
				#ifdef USE_CRYPTOGRAPHY
				// Accepts incoming connections and launches the sessions
				class RADJAV_EXPORT HashMultipart
				{
				public:
				  HashMultipart(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
				  ~HashMultipart();

				public:
				  
				  /// Read from a key in the database.
                                  void update(const void *text, int textLength, const std::string& inputEncoding,
						std::function <void (const std::string& str)> stringSetter,
						std::function <void (void* buf, int bufLen)> binSetter);
                                  void finalize(std::function <void (const std::string& str)> stringSetter,
						std::function <void (void* buf, int bufLen)> binSetter);
				  void reset();


				public:
				  std::shared_ptr<Engine::Crypto::IDigestMultipart> myDigest;

				  String myHashAlgorithm;
				  String myCryptoLibrary;
				  String myInputEncoding;
				  String myOutputEncoding;

				protected:
				  V8JavascriptEngine *jsEngine;

				  
				};
				#endif
			}
		}
	}
#endif

