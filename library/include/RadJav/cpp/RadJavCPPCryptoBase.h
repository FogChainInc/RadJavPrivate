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
	#include <boost/asio/ssl.hpp>
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
					 * @brief Base class for cryptography.
					 * @details Generic interface for cryptography routines.
					 */
					class RADJAV_EXPORT Base
					{
						public:
							#ifdef USE_V8
							Base(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
							#elif defined USE_JAVASCRIPTCORE
							Base(JSCJavascriptEngine *jsEngine, JSContextRef ctx, RJUINT argumentCount, const JSValueRef arguments[]);
							#endif
					
							~Base();
					
							void processInput(const void *plainText, int textLength,
											  std::string &decodedText,
											  const void *&binPlainText, int &binPlainTextLength);
							void processOutput(std::tuple<std::shared_ptr<void>, unsigned int> result,
											   std::function <void (const std::string& str)> stringSetter,
											   std::function <void (void* buf, int bufLen)> binSetter);

						public:
							String myCryptoLibrary;
							String myAlgorithm;
							String myInputEncoding;
							String myOutputEncoding;
					
						protected:
							#ifdef USE_V8
							V8JavascriptEngine
							#elif defined USE_JAVASCRIPTCORE
							JSCJavascriptEngine
							#endif
								*jsEngine;
					};

					/// Initialize SSL certificates.
					void initializeCertificates();
					/// Add an SSL certificate.
					void addCertificate(String certificate);
					/// Get a SSL certificate at an index.
					String getCertificate(RJINT index);
					/// Get all loaded SSL certificates.
					Array<String> *getCertificates();
					/// Get the default SSL certificates.
					Array<String> getDefaultCertificates();

					/// The SSL context.
					static boost::asio::ssl::context SSLContext{ boost::asio::ssl::context::sslv23_client };
					static Array<String> *SSLCertificates = NULL;
				#endif
			}
		}
	}
#endif

