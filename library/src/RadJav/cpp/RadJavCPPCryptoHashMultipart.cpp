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

#include "cpp/RadJavCPPCryptoHashMultipart.h"


#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>

#include "RadJavString.h"
#include "v8/RadJavV8JavascriptEngine.h"

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
			HashMultipart::HashMultipart(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			  //std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl;
			  std::cout << "Args len: " << args.Length() << std::endl;

			  this -> jsEngine = jsEngine;
			  v8::Isolate *isolate = args.GetIsolate();
			  
			  v8::Local<v8::Value> firstArg = args[0];


			  // Get constructor parms for HashMultipart(hashAlgorithm, cryptoLibrary)
			  if (args[0] -> IsString())
			    {
			      myHashAlgorithm = parseV8Value(args[0]);
			      myCryptoLibrary = parseV8Value(args[1]);
			    }

			  // Get constructor parms HashMultipart({hashAlgorithm: ..., cryptoLibrary: ..., ...})
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
			      myDigest = ORB::Engine::Crypto::createDigestMultipart(myHashAlgorithm, myCryptoLibrary);
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

			HashMultipart::~HashMultipart()
			{

			}



		        void HashMultipart::update(const void *text, int textLength, const std::string &inputEncoding,
						   std::function <void (const std::string &str)> stringSetter,
						   std::function <void (void* buf, int bufLen)> binSetter)
			{
			  //std::tuple<std::shared_ptr<void>, unsigned int> digestResult;
			  //std::cout << __PRETTY_FUNCTION__ << ": String" << std::endl << std::flush;
					    
			  
			  String _inputEncoding = inputEncoding;
			  if (_inputEncoding == "")
			    _inputEncoding = myInputEncoding;
			  
			  const void *binText;
			  int binTextLength;
			  std::string decodedText;

			  if (_inputEncoding == "binary")
			    {
			      binText = text;
			      binTextLength =  textLength;
			    }
			  else if (_inputEncoding == "hex")
 			    {
			      auto binData = ORB::Engine::Crypto::decodeHex(text, textLength);
			      decodedText.assign(static_cast<const char*>(std::get<0>(binData).get()), std::get<1>(binData));
			      binText = decodedText.c_str();
			      binTextLength = decodedText.length();
			    }
			  else if (_inputEncoding == "base64")
			    {
			      auto binData = ORB::Engine::Crypto::decodeBase64(text, textLength);
			      decodedText.assign(static_cast<const char*>(std::get<0>(binData).get()), std::get<1>(binData));
			      binText = decodedText.c_str();
			      binTextLength = decodedText.length();
			    }
			  else
			    {
			      String msg = "Unsupported input encoding: " + _inputEncoding;
			      throw std::invalid_argument(msg);
			    }
			  
			  myDigest -> update(binText, binTextLength);

			} // End of update()


		        void HashMultipart::finalize(std::function <void (const std::string& str)> stringSetter,
						     std::function <void (void* buf, int bufLen)> binSetter)
			{
			  //std::tuple<std::shared_ptr<void>, unsigned int> digestResult;
			  //std::cout << __PRETTY_FUNCTION__ << ": String" << std::endl << std::flush;
					    
			  auto digestResult = myDigest -> finalize();

			  if (myOutputEncoding == "hex")
			    {
			      std::cout << "Cipher string encoded as hex " << std::endl;
			      stringSetter(ORB::Engine::Crypto::encodeHex(std::get<0>(digestResult).get(),
									  std::get<1>(digestResult)));
			      return;
			    }
			  else if (myOutputEncoding == "base64")
			    {
			      std::cout << "Cipher string encoded as basee64 " << std::endl;
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

			} // End of finalize()


                  #endif
		  
		}
	}
}

