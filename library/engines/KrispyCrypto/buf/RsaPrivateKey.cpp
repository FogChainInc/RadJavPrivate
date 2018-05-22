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

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {
      RsaPrivateKey::RsaPrivateKey(std::unique_ptr<RSA> rsa, )
      {
	myRsa = std::move(rsa);
	myBits = bits;
      }

      RsaPrivateKey::~RsaPrivateKey()
      {
	BN_free(myBigNumExponent);
      }

      std::shared_ptr<IPrivateKey> RsaPrivateKey::generate(int bits)
      {

	auto rsa = std::unique_ptr<RSA>(RSA_new(), RSA_free);

	int ret = RSA_generate_key_ex(rsa.get(), bits, myBigNumExponent, NULL);
	if (ret != 1)
	  std::invalid_argument(std::string("Generating key pair failed for: ") + pubExponent);

	return ORB::Engine::Crypto::OpenSSL::createRsaPrivateKey(std::move(rsa));

      }

	  

    } // End of OpenSSL
    #endif 
  } // End of Crypto
} // End of Engine



