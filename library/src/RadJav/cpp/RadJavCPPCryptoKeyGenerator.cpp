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

#include "cpp/RadJavCPPCryptoKeyGenerator.h"

#include "RadJavString.h"
#include "v8/RadJavV8JavascriptEngine.h"

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
		        KeyGenerator::KeyGenerator(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args) :
			  Base(jsEngine, args)
			{
			  v8::Isolate *isolate = args.GetIsolate();
			  

			  // Get constructor parms for KeyGenerator(algorithm, cryptoLibrary)
			  if (args[0] -> IsString())
			    {
			      myAlgorithm = parseV8Value(args[0]);
			      myCryptoLibrary = parseV8Value(args[1]);
			    }

			  // Get constructor parms Cipher({cipherAlgorithm: ..., cryptoLibrary: ..., ...})
			  else if (args[0] -> IsObject())
			    {
			      v8::Local<v8::Object> parms = v8::Local<v8::Object>::Cast(args[0]);

			      myBits = jsEngine -> v8GetString(parms, "bits");
			      myEncryptPadding = jsEngine -> v8GetString(parms, "encryptPadding");
			      mySignatureType = jsEngine -> v8GetString(parms, "signatureType");
			    }
			  

			  // Defaults and error checking

			  // Create the key generator
			  try
			    {
			      std::map<std::string, std::string> parms;
			      parms["algorithm"] = myAlgorithm;
			      parms["bits"] = myBits;
			      parms["encryptPadding"] = myEncryptPadding;
			      parms["signatureType"] = mySignatureType;

			      myKeyGenerator = ORB::Engine::Crypto::createKeyGenerator(parms, myCryptoLibrary);


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

			  //std::cout << __PRETTY_FUNCTION__ << ": end" << std::endl;

			}

			KeyGenerator::~KeyGenerator()
			{

			}

		        std::shared_ptr<Engine::Crypto::IPrivateKey> KeyGenerator::generate()
			{

			  return myKeyGenerator -> generate();
			}
		    
		  
		  #endif
		  
		}
	}
}

