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

#ifndef _ORB_ENGINE_CRYPTO_OPENSSL_h_
#define _ORB_ENGINE_CRYPTO_OPENSSL_h_

#include <memory>
#include <string>
#include <map>
#include <i/engines/KrispyCrypto/IDigest.h>
#include <i/engines/KrispyCrypto/ICipher.h>
#include <i/engines/KrispyCrypto/IDecipher.h>

#include <i/engines/KrispyCrypto/IKeyPairGenerator.h>

//
// Note, this file pulls some OpenSSL include files. It defines unique pointers with custom deleters. Architecture would
// be simpler if we used shared_ptr, but we should use unique_ptr for handling private key information to ensure that
// it is not splattered all over the place.
//
#include "../src/OpenSSLCommonTypes.h"
#include <openssl/obj_mac.h>

namespace Engine
{
  namespace Crypto
  {
    namespace OpenSSL
    {
      class RsaKeyGenerator;
      class RsaPrivateKey;
      class RsaPublicKey;
    }
  }
}

namespace ORB
{
  namespace Engine
  {
    namespace Crypto
    {
      
      #ifdef USE_OPENSSL

      namespace OpenSSL
      {
	std::shared_ptr<::Engine::Crypto::IDigest> createDigest(const std::string &digestType);
	std::shared_ptr<::Engine::Crypto::IDigestMultipart> createDigestMultipart(const std::string &digestType);
	std::map<std::string, std::string> getListOfDigests();

	std::shared_ptr<::Engine::Crypto::ICipher> createCipher(const std::string &cipherType,
								const std::string &secret,
								const std::string &iv="");
	
	std::shared_ptr<::Engine::Crypto::ICipherMultipart> createCipherMultipart(const std::string &cipherType,
										  const std::string &secret,
										  const std::string &iv="");
	
	std::shared_ptr<::Engine::Crypto::IDecipher> createDecipher(const std::string &cipherType,
								    const std::string &secret,
								    const std::string &iv="");
	
	std::shared_ptr<::Engine::Crypto::IDecipherMultipart> createDecipherMultipart(const std::string &cipherType,
										      const std::string &secret,
										      const std::string &iv="");
	std::map<std::string, std::string> getListOfCiphers();

	//rsa-f4, rsa-3
	std::shared_ptr<::Engine::Crypto::OpenSSL::RsaKeyGenerator> createRsaKeyGenerator(int bits,
											  unsigned long pubExponent=RSA_F4,
											  int encryptPadding=RSA_PKCS1_PADDING,
											  int signatureType=NID_sha256);

	std::unique_ptr<::Engine::Crypto::OpenSSL::RsaPrivateKey> createRsaPrivateKey(::Engine::Crypto::OpenSSL::RsaStructUniquePtr rsa,
										      int bits,
										      const std::string &encryptPadding,
										      const std::string &signatureType);

	std::unique_ptr<::Engine::Crypto::OpenSSL::RsaPrivateKey> createRsaPrivateKey(::Engine::Crypto::OpenSSL::RsaStructUniquePtr rsa,
										      int bits,
										      int encryptPadding = RSA_PKCS1_PADDING,
										      int signatureType = NID_sha256);

	std::unique_ptr<::Engine::Crypto::OpenSSL::RsaPrivateKey> createRsaPrivateKey(const char *path, const char *format, const char *pwd,
										      int encryptPadding = RSA_PKCS1_PADDING,
										      int signatureType = NID_sha256);
	
	std::shared_ptr<::Engine::Crypto::OpenSSL::RsaPublicKey> createRsaPublicKey(::Engine::Crypto::OpenSSL::RsaStructUniquePtr rsa,
										    int bits,
										    int encryptPadding = RSA_PKCS1_PADDING,
										    int signatureType = NID_sha256);
	
	std::unique_ptr<::Engine::Crypto::OpenSSL::RsaPublicKey> createRsaPublicKey(const char *path, const char *format,
										    int encryptPadding = RSA_PKCS1_PADDING,
										    int signatureType = NID_sha256);

      } // End of OpenSSL
      
      #endif
      
    } // End of Crypto
  } // End of Engine
} // End of ORB

#endif // _ORB_ENGINE_CRYPTO_OPENSSL_h_p
