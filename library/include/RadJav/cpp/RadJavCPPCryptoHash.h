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
#ifndef _RADJAV_CPP_CRYPTO_HASH_H_
	#define _RADJAV_CPP_CRYPTO_HASH_H_

	#include "RadJavPreprocessor.h"

	#include <string>
	#include <memory>


	#include "RadJav.h"
	#include "RadJavString.h"
	#include "RadJavHashMap.h"
	#include "RadJavThread.h"

#ifdef USE_CRYPTOGRAPHY

#include <i/engines/KrispyCrypto/IDigest.h>

#endif 

	namespace RadJAV
	{
		namespace CPP
		{
			namespace Crypto
			{
				#ifdef USE_CRYPTOGRAPHY
				/**
				 * @ingroup group_crypto_cpp
				 * @brief Hash class.
				 */
				class RADJAV_EXPORT Hash
				{
				public:
					#ifdef USE_V8
				        /**
					 * @brief Constructs the object.
					 *
					 * @param jsEngine JavaScript engine object.
					 * @param args Array of arguments passed from JavaScript.
					 */
					Hash(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
					#elif defined USE_JAVASCRIPTCORE
				        /**
					 * @brief Constructs the object.
					 *
					 * @param jsEngine JavaScript engine object.
					 * @param ctx JavaScript engine context.
					 * @param argumentCount Number of arguments.
					 * @param arguments Argument array.
					 */
					Hash(JSCJavascriptEngine *jsEngine, JSContextRef ctx, RJUINT argumentCount, const JSValueRef arguments[]);
					#endif
					
				        /**
					 * @brief Destroys the object.
					 */
					~Hash();
					
				public:
					
					/**
					 * @brief Performs a digest.
					 *
					 * @param text Pointer to binary data.
					 * @param textLength Data size/length.
					 * @param stringSetter If a requested encoding resulted in a ASCII result, this setter will be used.
					 * @param binSetter If a requested encoding resulted in a binary result, this setter will be used.
					 */
					void digest(const void* text, int textLength,
								std::function <void (const std::string& str)> stringSetter,
								std::function <void (void* buf, int bufLen)> binSetter);
					
					
				public:
					/** @name KrispyCrypto object */
					//@{
					std::shared_ptr<Engine::Crypto::IDigest> myDigest;
					//@}

					/** @name Parameters */
					//@{
					String myHashAlgorithm;
					String myCryptoLibrary;
					String myInputEncoding;
					String myOutputEncoding;
					//@}
					
				protected:
					#ifdef USE_V8
					/** @name JavaScript engine object */
					V8JavascriptEngine
					#elif defined USE_JAVASCRIPTCORE
					/** @name JavaScript engine object */
					JSCJavascriptEngine
					#endif
						*jsEngine;
				};
				#endif
			}
		}
	}
#endif

