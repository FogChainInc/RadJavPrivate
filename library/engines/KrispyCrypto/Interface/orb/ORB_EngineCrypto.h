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

#ifndef _ORB_ENGINE_CRYPTO_h_
#define _ORB_ENGINE_CRYPTO_h_

#include <memory>
#include <string>
#include <map>
#include <tuple>

namespace Engine
{
  namespace Crypto
  {
    class IDigest;
    class IDigestMultipart;
    class ICipher;
    class ICipherMultipart;
    class IDecipher;
    class IDecipherMultipart;
    class IKeyGenerator;
    class IPrivateKey;
    class IPublicKey;
  }
}

/**
 * @addtogroup ORB
 * 
 * @brief KrispyCrypto Object Request Broker top level.
 *
 * Object Request Broker is a layered structure of calls assisting in proper object creation. The top layer provides code from outside
 * of KrispyCrypto with a way to create KrispyCrypto objects.
 *
 * Note, each library implementation (OpenSSL, for example), shall provide a sencond ORB layer. This way, the ORB top level implementation simply delegates
 * to specific library ORB, and doesn't need to know too much detail.
 */

namespace ORB
{
  namespace Engine
  {
    namespace Crypto
    {

      /**
       * @brief Creates a Digest object.
       *
       * @param digestType A string repreenting the digest algorithm to be used.
       * @param cryptoLibrary A string representing crypto library to be used.
       *
       * @returns the digest object.
       *
       * Exception will be thrown if ORB cannot find either the requested library or algorithm.
       */
      std::shared_ptr<::Engine::Crypto::IDigest>
	createDigest(const std::string &digestType,
		     const std::string &cryptoLibrary = "OpenSSL");

      /**
       * @brief Creates a DigestMultipart object.
       *
       * @param digestType A string repreenting the digest algorithm to be used.
       * @param cryptoLibrary A string representing crypto library to be used.
       *
       * @returns the digest object.
       *
       * Exception will be thrown if ORB cannot find either the requested library or algorithm.
       */
      std::shared_ptr<::Engine::Crypto::IDigestMultipart>
	createDigestMultipart(const std::string &digestType,
			      const std::string &cryptoLibrary = "OpenSSL");
      
      /**
       * @brief Returns a list of supported digests.
       *
       * @param cryptoLibrary A string representing crypto library to be used.
       *
       * @returns a map of key (digest string/tag) <-> value (human readable description of the algorithm) pair.
       *
       * Exception will be thrown if ORB cannot find either the requested library.
       */
      std::map<std::string, std::string> getListOfDigests(const std::string &cryptoLibrary = "OpenSSL");


      /**
       * @brief Creates a Cipher object.
       *
       * @param cipherType A string repreenting the cipher algorithm to be used.
       * @param secret A string repreenting the password/secred used for encryption key generation.
       * @param iv A string representing initialization vector.
       * @param cryptoLibrary A string representing crypto library to be used.
       *
       * @returns the cipher object.
       *
       * Exception will be thrown if ORB cannot find either the requested cipher type or library.
       */
      std::shared_ptr<::Engine::Crypto::ICipher> createCipher(const std::string &cipherType,
							      const std::string &secret,
							      const std::string &iv="",
							      const std::string &cryptoLibrary = "OpenSSL");
	
      /**
       * @brief Creates a CipherMultipart object.
       *
       * @param cipherType A string repreenting the cipher algorithm to be used.
       * @param secret A string repreenting the password/secred used for encryption key generation.
       * @param iv A string representing initialization vector.
       * @param cryptoLibrary A string representing crypto library to be used.
       *
       * @returns the cipher object.
       *
       * Exception will be thrown if ORB cannot find either the requested cipher type or library.
       */
      std::shared_ptr<::Engine::Crypto::ICipherMultipart> createCipherMultipart(const std::string& cipherType,
										const std::string &secret,
										const std::string &iv="",
										const std::string &cryptoLibrary = "OpenSSL");
	
      /**
       * @brief Creates a Decipher object.
       *
       * @param cipherType A string repreenting the cipher algorithm to be used.
       * @param secret A string repreenting the password/secred used for encryption key generation.
       * @param iv A string representing initialization vector.
       * @param cryptoLibrary A string representing crypto library to be used.
       *
       * @returns the decipher object.
       *
       * Exception will be thrown if ORB cannot find either the requested cipher type or library.
       */
      std::shared_ptr<::Engine::Crypto::IDecipher> createDecipher(const std::string& cipherType,
								  const std::string &secret,
								  const std::string &iv="",
								  const std::string &cryptoLibrary = "OpenSSL");
	
      /**
       * @brief Creates a DecipherMultipart object.
       *
       * @param cipherType A string repreenting the cipher algorithm to be used.
       * @param secret A string repreenting the password/secred used for encryption key generation.
       * @param iv A string representing initialization vector.
       * @param cryptoLibrary A string representing crypto library to be used.
       *
       * @returns the decipher object.
       *
       * Exception will be thrown if ORB cannot find either the requested cipher type or library.
       */
      std::shared_ptr<::Engine::Crypto::IDecipherMultipart> createDecipherMultipart(const std::string& cipherType,
										    const std::string &secret,
										    const std::string &iv="",
										    const std::string &cryptoLibrary = "OpenSSL");

      /**
       * @brief Returns a list of supported ciphers.
       *
       * @param cryptoLibrary A string representing crypto library to be used.
       *
       * @returns a map of key (cipher string/tag) <-> value (human readable description of the algorithm) pair.
       *
       * Exception will be thrown if ORB cannot find either the requested cipher type or library.
       */
      std::map<std::string, std::string> getListOfCiphers(const std::string &cryptoLibrary = "OpenSSL");

      #define OPENSSL_DEFAULT_BITS "2048"
      #define OPENSSL_DEFAULT_PUB_EXPONENT "RSA_F4"
      #define OPENSSL_DEFAULT_ENCRYPT_PADDING "RSA_PKCS11_PADDING"
      #define OPENSSL_DEFAULT_SIGNATURE_TYPE "OPENSSL_DEFAULT"

      //rsa-f4, rsa-3
      /**
       * @brief Returns a RSA key generator.
       *
       */
      std::shared_ptr<::Engine::Crypto::IKeyGenerator> createRsaKeyGenerator(int bits=2048,
									     std::string pubExponent="RSA_F4",
									     std::string encryptPadding="RSA_PKCS1_PADDING",
									     std::string signatureType="OPENSSL_DEFAULT");

      /**
       * @brief Creates a key generator.
       *
       * @param parms A list of key <-> value pairs telling the library how to create the generator.
       * @param cryptoLibrary A string representing the crypto library.
       *
       */
      std::shared_ptr<::Engine::Crypto::IKeyGenerator> createKeyGenerator(std::map<std::string, std::string> parms,
									  std::string cryptoLibrary="OpenSSL");


      /**
       * @brief Creates private key object from external data.
       *
       * @param parms A list of key <-> value pairs telling the library about the location and format of key data..
       * @param cryptoLibrary A string representing the crypto library.
       *
       */
      std::shared_ptr<::Engine::Crypto::IPrivateKey> createPrivateKey(std::map<std::string, std::string> parms,
								      std::string cryptoLibrary="OpenSSL");


      /**
       * @brief Creates public key object from external data.
       *
       * @param parms A list of key <-> value pairs telling the library about the location and format of key data..
       * @param cryptoLibrary A string representing the crypto library.
       *
       */
      std::shared_ptr<::Engine::Crypto::IPublicKey> createPublicKey(std::map<std::string, std::string> parms,
								    std::string cryptoLibrary="OpenSSL");
      

      /**
       * @brief Encodes data into a string containing hex representation.
       *
       * @param data A pointer to the binary data.
       * @param dataLen Lenth/size of the data in the buffer.
       *
       * @returns encoded string.
       *
       */
      std::string encodeHex(const void* data, int dataLen);

      /**
       * @brief Dencodes a string containing hex representation into binary data.
       *
       * @param A string containing hex representation of data.
       *
       * @returns a tuple containing buffer and the data size. Caller owns the resource now.
       *
       */
      std::tuple<std::shared_ptr<void>, unsigned int> decodeHex(const std::string &hexString);

      /**
       * @brief Dencodes a string containing hex representation into binary data.
       *
       * @param data A pointer to the binary data.
       * @param dataLen Lenth/size of the data in the buffer.
       *
       * @returns a tuple containing buffer and the data size. Caller owns the resource now.
       *
       */
      std::tuple<std::shared_ptr<void>, unsigned int> decodeHex(const void *data, int dataLength);

      /**
       * @brief Encodes data into a string containing base64 representation.
       *
       * @param data A pointer to the binary data.
       * @param dataLen Lenth/size of the data in the buffer.
       *
       * @returns encoded string.
       *
       */
      std::string encodeBase64(const void* data, int dataLen);

      /**
       * @brief Dencodes a string containing base64 representation into binary data.
       *
       * @param A string containing hex representation of data.
       *
       * @returns a tuple containing buffer and the data size. Caller owns the resource now.
       *
       */
      std::tuple<std::shared_ptr<void>, unsigned int> decodeBase64(const std::string &base64String);

      /**
       * @brief Dencodes a string containing hex representation into base64 data.
       *
       * @param data A pointer to the binary data.
       * @param dataLen Lenth/size of the data in the buffer.
       *
       * @returns a tuple containing buffer and the data size. Caller owns the resource now.
       *
       */
      std::tuple<std::shared_ptr<void>, unsigned int> decodeBase64(const void *data, int dataLength);

    } // End of Crypto
  } // End of Engine
} // End of ORB

#endif // _ORB_ENGINE_CRYPTO_h_
