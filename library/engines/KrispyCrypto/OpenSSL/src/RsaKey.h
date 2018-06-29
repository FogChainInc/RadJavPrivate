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

#ifndef _ENGINE_CRYPTO_OPENSSL_RSAKEY_h_
#define _ENGINE_CRYPTO_OPENSSL_RSAKEY_h_

#include <i/engines/KrispyCrypto/IPrivateKey.h>

#include "OpenSSLCommonTypes.h"

#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>

#include <string>

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {
      class RsaKey : virtual public IKey
      {

      public:
	RsaKey(RsaStructUniquePtr rsa, int bits, int encryptPadding, int signatureType);
	RsaKey(RsaStructUniquePtr rsa, int bits);
	RsaKey(int encryptPadding, int signatureType);
	//	RsaKey();
	virtual ~RsaKey();
      

	//void setBits(int bits);
      protected:
	RsaStructUniquePtr myRsa;
	EvpPkeyUniquePtr myEvpPkey;
	int myBits;

	int myEncryptPadding;
	int mySignatureType;
      };
    } // End of OpenSSL
    #endif 
  } // End of Crypto
} // End of Engine



#endif // _ENGINE_CRYPTO_OPENSSL_RSAKEY_h_
