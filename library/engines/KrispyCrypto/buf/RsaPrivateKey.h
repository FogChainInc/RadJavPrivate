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

#include <i/Engine/Crypto/IPrivateKey.h>
#include <openssl/rsa.h>

#include <string>

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {
      class RsaKeyGenerator : virtual public IPrivateKey
      {

      public:
	RsaPrivateKey(std::unique_ptr<RSA>rsa, int bits);
	virtual ~RsaKeyGenerator();
      
	virtual std::tuple<std::shared_ptr<void>, unsigned int>
	  encrypt(const unsigned char* data, unsigned int dataLength, int padding=0);
	virtual std::tuple<std::shared_ptr<void>, unsigned int>
	  decrypt(const unsigned char* data, unsigned int dataLength, int padding=0);

      
      private:
	std::unique_ptr<RSA> myRsa;
	unsigned long myExponent;

	std::unique_ptr<BIO> myPublicKey;
	std::unique_ptr<BIO> myPrivateKey;
	
      };
    } // End of OpenSSL
    #endif 
  } // End of Crypto
} // End of Engine



#endif // _ENGINE_CRYPTO_OPENSSL_RSAPRIVATEKEY_h_
