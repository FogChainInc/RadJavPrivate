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
#include "RsaPrivateKey.h"

#include "RsaKeyGenerator.h"

#include "OpenSSLCommonTypes.h" // RsaStructUniquePtr
#include "../orb/ORB_EngineCryptoOpenSSL.h"

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {
            
      RsaKeyGenerator::RsaKeyGenerator(int bits, unsigned long pubExponent, int encryptPadding, int signatureType) :
	myBits(bits),
	myBigNumExponent(BN_new(), BN_free),
	myPubExponent(pubExponent),
	myEncryptPadding(encryptPadding),
	mySignatureType(signatureType)
      {
	int ret =  BN_set_word(myBigNumExponent.get(), pubExponent);
	if (ret != 1)
	  std::invalid_argument(std::string("Creating public exponent failed for: ") +
				std::to_string(pubExponent));
      }

      RsaKeyGenerator::~RsaKeyGenerator()
      {
      }

      std::shared_ptr<IPrivateKey> RsaKeyGenerator::generate()
      {

	RsaStructUniquePtr rsa(RSA_new(), RSA_free);

	int ret = RSA_generate_key_ex(rsa.get(), myBits, myBigNumExponent.get(), NULL);
	if (ret != 1)
	  throw(std::invalid_argument(std::string("Generating key pair failed for: ") + std::to_string(myPubExponent)));

	return ORB::Engine::Crypto::OpenSSL::createRsaPrivateKey(std::move(rsa), myBits,
								 myEncryptPadding, mySignatureType);

      }

    } // End of OpenSSL
    #endif 
  } // End of Crypto
} // End of Engine



