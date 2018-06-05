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
#include "../orb/ORB_EngineCryptoOpenSSL.h"

#include "../src/RsaKeyGenerator.h"
#include "../src/RsaPrivateKey.h"
#include "../src/RsaPublicKey.h"


namespace ORB
{
  namespace Engine
  {
    namespace Crypto
    {
      
      #ifdef USE_OPENSSL
      
      namespace OpenSSL
      {

	std::shared_ptr<::Engine::Crypto::OpenSSL::RsaKeyGenerator> createRsaKeyGenerator(int bits,
											  unsigned long pubExponent,
											  int encryptPadding,
											  int signatureType)
	{
	  /*
	  unsigned long pubExponent;
	  
	  if (pubExponentStr == "rsa-3")
	    pubExponent = RSA_3;
	  else if (pubExponentStr == "rsa-f4")
	    pubExponent = RSA_F4;
	  else throw std::invalid_argument(std::string("Unsupported public exponent: ") + std::to_string(pubExponent));
	  */

	  return std::make_shared<::Engine::Crypto::OpenSSL::RsaKeyGenerator>(bits, pubExponent, encryptPadding, signatureType);
	}

	std::unique_ptr<::Engine::Crypto::OpenSSL::RsaPrivateKey> createRsaPrivateKey(::Engine::Crypto::OpenSSL::RsaStructUniquePtr rsa,
										      int bits,
										      int encryptPadding,
										      int signatureType
										      )
	{

	  return std::unique_ptr<::Engine::Crypto::OpenSSL::RsaPrivateKey>
	    (new ::Engine::Crypto::OpenSSL::RsaPrivateKey(std::move(rsa), bits, encryptPadding, signatureType));
	}
	
	std::shared_ptr<::Engine::Crypto::OpenSSL::RsaPublicKey> createRsaPublicKey(::Engine::Crypto::OpenSSL::RsaStructUniquePtr rsa,
										      int bits,
										      int encryptPadding,
										      int signatureType
										      )
	{

	  return std::make_shared<::Engine::Crypto::OpenSSL::RsaPublicKey>(std::move(rsa), bits, encryptPadding, signatureType);
	}
	

      } // End of OpenSSL
      
      #endif // USE_OPENSSL
      
      
    } // End of Crypto
  } // End of Engine
} // End of ORB
