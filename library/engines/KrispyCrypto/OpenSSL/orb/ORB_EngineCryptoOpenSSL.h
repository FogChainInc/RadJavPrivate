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
	/**
	 * @brief Creates a Digest object.
	 *
	 * @param digestType A string repreenting the digest algorithm to be used.
	 *
	 * @returns the digest object.
	 *
	 * Exception will be thrown if ORB cannot find either the requested library or algorithm.
	 */
	std::shared_ptr<::Engine::Crypto::IDigest> createDigest(const std::string &digestType);
	
	/**
	 * @brief Creates a DigestMultipart object.
	 *
	 * @param digestType A string repreenting the digest algorithm to be used.
	 *
	 * @returns the digest object.
	 *
	 * Exception will be thrown if ORB cannot find either the requested library or algorithm.
	 */
	std::shared_ptr<::Engine::Crypto::IDigestMultipart> createDigestMultipart(const std::string &digestType);

	
	/**
	 * @brief Returns a list of supported digests.
	 *
	 * @returns a map of key (digest string/tag) <-> value (human readable description of the algorithm) pair.
	 *
	 * Exception will be thrown if ORB cannot find either the requested library.
	 */
	std::map<std::string, std::string> getListOfDigests();

	
	/**
	 * @brief Creates a Cipher object.
	 *
	 * @param cipherType A string repreenting the cipher algorithm to be used.
	 * @param secret A string repreenting the password/secred used for encryption key generation.
	 * @param iv A string representing initialization vector.
	 *
	 * @returns the cipher object.
	 *
	 * Exception will be thrown if ORB cannot find either the requested cipher type or library.
	 */
	std::shared_ptr<::Engine::Crypto::ICipher> createCipher(const std::string &cipherType,
								const std::string &secret,
								const std::string &iv="");
	
	/**
	 * @brief Creates a CipherMultipart object.
	 *
	 * @param cipherType A string repreenting the cipher algorithm to be used.
	 * @param secret A string repreenting the password/secred used for encryption key generation.
	 * @param iv A string representing initialization vector.
	 *
	 * @returns the cipher object.
	 *
	 * Exception will be thrown if ORB cannot find either the requested cipher type or library.
	 */
	std::shared_ptr<::Engine::Crypto::ICipherMultipart> createCipherMultipart(const std::string &cipherType,
										  const std::string &secret,
										  const std::string &iv="");
	
	/**
	 * @brief Creates a Decipher object.
	 *
	 * @param cipherType A string repreenting the cipher algorithm to be used.
	 * @param secret A string repreenting the password/secred used for encryption key generation.
	 * @param iv A string representing initialization vector.
	 *
	 * @returns the decipher object.
	 *
	 * Exception will be thrown if ORB cannot find either the requested cipher type or library.
	 */
	std::shared_ptr<::Engine::Crypto::IDecipher> createDecipher(const std::string &cipherType,
								    const std::string &secret,
								    const std::string &iv="");
	
	/**
	 * @brief Creates a DecipherMultipart object.
	 *
	 * @param cipherType A string repreenting the cipher algorithm to be used.
	 * @param secret A string repreenting the password/secred used for encryption key generation.
	 * @param iv A string representing initialization vector.
	 *
	 * @returns the decipher object.
	 *
	 * Exception will be thrown if ORB cannot find either the requested cipher type or library.
	 */
	std::shared_ptr<::Engine::Crypto::IDecipherMultipart> createDecipherMultipart(const std::string &cipherType,
										      const std::string &secret,
										      const std::string &iv="");
	/**
	 * @brief Returns a list of supported ciphers.
	 *
	 * @returns a map of key (cipher string/tag) <-> value (human readable description of the algorithm) pair.
	 *
	 * Exception will be thrown if ORB cannot find either the requested cipher type or library.
	 */
	std::map<std::string, std::string> getListOfCiphers();

	/**
	 * @brief Creates a RSA key generator.
	 *
	 * @param pubExponent specifies which public exponent to use: RSA_F4, or RSA_F3.
	 * @param encryptPadding specifies if padding should be used (most of the time yes).
	 * @param signatureType
	 *
	 * @returns the key generator object.
	 */
	std::shared_ptr<::Engine::Crypto::OpenSSL::RsaKeyGenerator> createRsaKeyGenerator(int bits,
											  unsigned long pubExponent=RSA_F4,
											  int encryptPadding=RSA_PKCS1_PADDING,
											  int signatureType=NID_sha256);

	/**
	 * @brief Creates RSA private key from RSA structure.
	 *
	 * @param rsa A pointer to RSA structure.
	 * @param bits Length of the generatedkey.
	 * @param encryptPadding Should the padding be used (most of the time yes).
	 * @param signatureType
	 *
	 * @returns the private key object.
	 *
	 * This is a higher level funciton, which is intended to be used by other modules, hence it doesn't depend on OpenSSL
	 * constants directly.
	 */
	std::unique_ptr<::Engine::Crypto::OpenSSL::RsaPrivateKey> createRsaPrivateKey(::Engine::Crypto::OpenSSL::RsaStructUniquePtr rsa,
										      int bits,
										      const std::string &encryptPadding,
										      const std::string &signatureType);

	/**
	 * @brief Creates RSA private key from RSA structure.
	 *
	 * @param rsa A pointer to RSA structure.
	 * @param bits Length of the generatedkey.
	 * @param encryptPadding Should the padding be used (most of the time yes).
	 * @param signatureType
	 *
	 * @returns the private key object.
	 */
	std::unique_ptr<::Engine::Crypto::OpenSSL::RsaPrivateKey> createRsaPrivateKey(::Engine::Crypto::OpenSSL::RsaStructUniquePtr rsa,
										      int bits,
										      int encryptPadding = RSA_PKCS1_PADDING,
										      int signatureType = NID_sha256);

	/**
	 * @brief Creates RSA private key from file.
	 *
	 * @param path The path to the file.
	 * @param format A format of the file.
	 * @param encryptPadding 
	 * @param signatureType
	 *
	 * @returns the private key object.
	 */
	std::unique_ptr<::Engine::Crypto::OpenSSL::RsaPrivateKey> createRsaPrivateKey(const char *path, const char *format, const char *pwd,
										      int encryptPadding = RSA_PKCS1_PADDING,
										      int signatureType = NID_sha256);
	
	/**
	 * @brief Creates RSA public key from RSA structure.
	 *
	 * @param rsa A pointer to RSA structure.
	 * @param bits Length of the generatedkey.
	 * @param encryptPadding Should the padding be used (most of the time yes).
	 * @param signatureType
	 *
	 * @returns the public key object.
	 */
	std::shared_ptr<::Engine::Crypto::OpenSSL::RsaPublicKey> createRsaPublicKey(::Engine::Crypto::OpenSSL::RsaStructUniquePtr rsa,
										    int bits,
										    int encryptPadding = RSA_PKCS1_PADDING,
										    int signatureType = NID_sha256);
	
	/**
	 * @brief Creates RSA public key from file.
	 *
	 * @param path The path to the file.
	 * @param format A format of the file.
	 * @param encryptPadding 
	 * @param signatureType
	 *
	 * @returns the public key object.
	 */
	std::unique_ptr<::Engine::Crypto::OpenSSL::RsaPublicKey> createRsaPublicKey(const char *path, const char *format,
										    int encryptPadding = RSA_PKCS1_PADDING,
										    int signatureType = NID_sha256);

      } // End of OpenSSL
      
#endif
      
    } // End of Crypto
  } // End of Engine
} // End of ORB

#endif // _ORB_ENGINE_CRYPTO_OPENSSL_h_p
