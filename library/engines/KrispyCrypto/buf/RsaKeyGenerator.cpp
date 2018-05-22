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

#include "RsaKeyGenerator.h"

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {
      RsaKeyGenerator::RsaKeyGenerator(unsigned long pubExponent)
      {
	if (pubExponent == "rsa-3")
	  myExponent = RSA_3;
	else if (pubExponent == "rsa-f4")
	  myExponent = RSA_F4;
	else throw std::invalid_argument(std::string("Unsupported public exponent: ") + pubExponent);

	myBigNumExponent = BN_new();

	int ret =  BN_set_word(myBigNumExponent, myExponent);
	if (ret != 1)
	  std::invalid_argument(std::string("Creating public exponent failed for: ") + pubExponent);
      }

      RsaKeyGenerator::~RsaKeyGenerator()
      {
	BN_free(myBigNumExponent);
      }

      std::shared_ptr<IPrivateKey> RsaKeyGenerator::generate(unsigned int bits)
      {

	auto rsa = std::unique_ptr<RSA>(RSA_new(), RSA_free);

	int ret = RSA_generate_key_ex(rsa.get(), bits, myBigNumExponent, NULL);
	if (ret != 1)
	  std::invalid_argument(std::string("Generating key pair failed for: ") + pubExponent);

	return ORB::Engine::Crypto::OpenSSL::createRsaPrivateKey(std::move(rsa));
	return nullptr;

      }

	  

    } // End of OpenSSL
    #endif 
  } // End of Crypto
} // End of Engine



