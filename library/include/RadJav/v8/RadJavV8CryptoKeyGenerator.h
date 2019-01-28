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
#ifndef _RADJAV_GUI_V8_CRYPTO_KEYGENERATOR_H_
	#define _RADJAV_GUI_V8_CRYPTO_KEYGENERATOR_H_

	#include "RadJavPreprocessor.h"

	#include <v8.h>

	namespace RadJAV
	{
		namespace V8B
		{
			/// RadJav Cryptography
			namespace Crypto
			{
				/**
				 * @ingroup group_crypto_js_v8
				 * @brief KeyGenerator callbacks.
				 * @details Class representing Javascript bindings to CPP::Crypto::KeyGenerator.
				 */
				class RADJAV_EXPORT KeyGenerator
				{
					public:
				                /**
						 * @brief Invoked during initialization, creates V8 callbacks.
						 */
						static void createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object);

				                /**
						 * @brief Implements javascript constructor.
						 */
						static void _init(const v8::FunctionCallbackInfo<v8::Value> &args);

				                /**
						 * @brief Implements javascript generate.
						 */
						static void generate(const v8::FunctionCallbackInfo<v8::Value> &args);

				};
			}
		}
	}
#endif

