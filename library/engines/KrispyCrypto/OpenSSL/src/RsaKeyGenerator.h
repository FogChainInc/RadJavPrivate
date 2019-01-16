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

#ifndef _ENGINE_CRYPTO_OPENSSL_RSAKEYGENERATOR_h_
#define _ENGINE_CRYPTO_OPENSSL_RSAKEYGENERATOR_h_

#include <i/engines/KrispyCrypto/IKeyGenerator.h>
#include <openssl/rsa.h>

#include <string>

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {
      /**
       * @class KeyGenerator
       *
       * @brief Generates private/public key pairs.
       *
       * Note, public key can be obtained from a private key object.
       */
      class RsaKeyGenerator : virtual public IKeyGenerator
      {
	using BigNumUniquePtr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;
	
      public:
	/**
	 * @brief Constructs a RsaKeyGenerator object.
	 *
	 * @param bits Length of the key.
	 * @param encryptPadding Should encrypt padding be used (usually yes).
	 * @param signatureType.
	 */
	RsaKeyGenerator(int bits, unsigned long pubExponent, int encryptPadding, int signatureType);

	/**
	 * Destroys the object.
	 */
	virtual ~RsaKeyGenerator();
      
	/**
	 * @brief Generates another key pair.
	 *
	 * @returns the key pair object (public key can be always obtained from the private key object).
	 */
	virtual std::shared_ptr<IPrivateKey> generate();
      
      private:
	/** @name Key parameters */
	//@{
	int myBits;
	  
	BigNumUniquePtr myBigNumExponent;
	unsigned long myPubExponent;
	//@}

	
	/** @name encryption parameters */
	//@{
	int myEncryptPadding;
	int mySignatureType;
	//@}
	  
      };
    } // End of OpenSSL
#endif 
  } // End of Crypto
} // End of Engine



#endif // _ENGINE_CRYPTO_OPENSSL_RSAKEYGENERATOR_h_
