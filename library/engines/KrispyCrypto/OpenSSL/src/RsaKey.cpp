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

#include "RsaKey.h"

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {

      RsaKey::RsaKey(RsaStructUniquePtr rsa, int bits, int encryptPadding, int signatureType) :
	myRsa(std::move(rsa)),
	myEvpPkey(EVP_PKEY_new(), EVP_PKEY_free),
	myBits(bits),
	myEncryptPadding(encryptPadding),
	mySignatureType(signatureType)
      {
	EVP_PKEY_set1_RSA(myEvpPkey.get(), myRsa.get());
      }

      RsaKey::RsaKey(RsaStructUniquePtr rsa, int bits) :
	myRsa(std::move(rsa)),
	myEvpPkey(EVP_PKEY_new(), EVP_PKEY_free),
	myBits(bits),
	myEncryptPadding(0),
	mySignatureType(0)
      {
	EVP_PKEY_set1_RSA(myEvpPkey.get(), myRsa.get());
      }

      RsaKey::RsaKey(int encryptPadding, int signatureType) :
	myRsa(RSA_new(), RSA_free),
	myEvpPkey(EVP_PKEY_new(), EVP_PKEY_free),
	myBits(0),
	myEncryptPadding(encryptPadding),
	mySignatureType(signatureType)
      {

      }
      
      
      RsaKey::~RsaKey()
      {

      }


    } // End of OpenSSL
    #endif 
  } // End of Crypto
} // End of Engine



