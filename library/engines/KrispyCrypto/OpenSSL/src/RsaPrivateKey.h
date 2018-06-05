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

#ifndef _ENGINE_CRYPTO_OPENSSL_RSAPRIVATEKEY_h_
#define _ENGINE_CRYPTO_OPENSSL_RSAPRIVATEKEY_h_

#include "RsaKey.h"
#include "RsaPublicKey.h"

#include "OpenSSLCommonTypes.h"

#include <i/engines/KrispyCrypto/IPrivateKey.h>

#include <openssl/rsa.h>
#include <openssl/bio.h>

#include <string>

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {
      class RsaPublicKey;

      class RsaPrivateKey : virtual public IPrivateKey, virtual public RsaKey
      {

      public:
	RsaPrivateKey(RsaStructUniquePtr rsa, int bits, int encryptPadding, int signatureType);
	//RsaPrivateKey(RsaStructUniquePtr rsa, int bits);
	virtual ~RsaPrivateKey();
      
	virtual std::shared_ptr<const IPublicKey> getPublicKey();
	virtual std::tuple<std::shared_ptr<void>, unsigned int> sign(const unsigned char* message,
								     unsigned int messageLength);

	virtual std::tuple<std::shared_ptr<void>, unsigned int>
	  encrypt(const unsigned char* message, unsigned int messageLength) const;
	virtual std::tuple<std::shared_ptr<void>, unsigned int>
	  decrypt(const unsigned char* message, unsigned int messageLength) const;

	void init(RsaStructUniquePtr &&rsa, int bits);
      private:
	
	std::shared_ptr<RsaPublicKey> myPublicKey;

	//	std::unique_ptr<BIO, decltype(&::BIO_free)> myPublicKey;
	//	std::unique_ptr<BIO, decltype(&::BIO_free)> myPrivateKey;
	
      };
    } // End of OpenSSL
    #endif 
  } // End of Crypto
} // End of Engine



#endif // _ENGINE_CRYPTO_OPENSSL_RSAPRIVATEKEY_h_
