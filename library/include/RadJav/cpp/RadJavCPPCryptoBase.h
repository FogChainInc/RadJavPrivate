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
					                /**
							 * @brief Constructs the object.
							 *
							 * @param jsEngine JavaScript engine object.
							 * @param args Array of arguments passed from JavaScript.
							 */
							Base(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
							#elif defined USE_JAVASCRIPTCORE
							/**
							 * @brief Constructs the object.
							 *
							 * @param jsEngine JavaScript engine object.
							 * @param ctx JavaScript engine context.
							 * @param argumentCount Number of arguments.
							 * @param arguments Argument array.
							 */
							Base(JSCJavascriptEngine *jsEngine, JSContextRef ctx, RJUINT argumentCount, const JSValueRef arguments[]);
							#endif
							
							/**
							 * @brief Destroys the object.
							 */
							~Base();
					
							/**
							 * @brief Processes input into from its encoding into a binary format.
							 *
							 * @param plainText A pointer to input data.
							 * @param plainTextLength Data size/length.
							 * @param stringSetter If a requested encoding resulted in a ASCII result, this setter will be used.
							 * @param binSetter If a requested encoding resulted in a binary result, this setter will be used.
							 * @param binPlainText An output argument, will contain the pointer to binary plain text.
							 * @param binPlainTextLength An output argument, will contain binary buffer size.
							 */
							void processInput(const void *plainText, int textLength,
											  std::string &decodedText,
											  const void *&binPlainText, int &binPlainTextLength);
							
							/**
							 * @brief Processes output binary data into a requested encoding.
							 *
							 * @param result A tuple containing a pointer to binary data and data size/length.
							 * @param stringSetter If a requested encoding resulted in a ASCII result, this setter will be used.
							 * @param binSetter If a requested encoding resulted in a binary result, this setter will be used.
							 */
							void processOutput(std::tuple<std::shared_ptr<void>, unsigned int> result,
											   std::function <void (const std::string& str)> stringSetter,
											   std::function <void (void* buf, int bufLen)> binSetter);

						public:
							/** @name Parameters */
							//@{
							String myCryptoLibrary;
							String myAlgorithm;
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

					//@{
					/**
					 * @brief Initializes SSL certificates.
					 */
					void initializeCertificates();

					//@{
					/**
					 * @brief Destroys the SSL certificates.
					 */
					void destroyCertificates();
					
					/**
					 * @brief Add an SSL certificate.
					 */
					void addCertificate(String certificate);

					/**
					 * @brief Get a SSL certificate at an index.
					 */
					String getCertificate(RJINT index);

					/**
					 * @brief Get all loaded SSL certificates.
					 */
					Array<String> *getCertificates();

					/**
					 * @brief Get the default SSL certificates.
					 */
					Array<String> getDefaultCertificates();
					//@}

					/**
					 * @brief The SSL context.
					 */
					static boost::asio::ssl::context SSLContext{ boost::asio::ssl::context::sslv23_client };

					/**
					 * Default cert storage.
					 */
					static Array<String> *SSLCertificates = NULL;
				#endif
			}
		}
	}
#endif

