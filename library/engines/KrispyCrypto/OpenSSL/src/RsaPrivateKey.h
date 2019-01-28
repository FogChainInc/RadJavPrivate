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

      /**
       * @class RsaPrivateKey
       *
       * @brief Implements IPrivateKey interface.
       */
      class RsaPrivateKey : virtual public IPrivateKey, virtual public RsaKey
      {

      public:
	/**
	 * @brief Constructs a RSA private keyt from RSA structure
	 *
	 * @param rsa RSA structure.
	 * @param encryptPadding Should encrypt padding be used (usually yes).
	 * @param signatureType.
	 */
	RsaPrivateKey(RsaStructUniquePtr rsa, int bits, int encryptPadding, int signatureType);
	
	/**
	 * @brief Constructs RSA private key from file.
	 *
	 * @param path The path to the file.
	 * @param format A format of the file.
	 * @param encryptPadding 
	 * @param signatureType
	 */
	RsaPrivateKey(const char *path, const char *format, const char *pwd, int encryptPadding, int signatureType);

	/**
	 * @brief Destroys the object.
	 */
	virtual ~RsaPrivateKey();

	/**
	 * @brief Provides access to the public key
	 *
	 * @returns A public key object
	 */
	virtual std::shared_ptr<const IPublicKey> getPublicKey();

	/**
	 * @brief Signs data with the key.
	 *
	 * @param data Pointer to the buffer containing data.
	 * @param dataLen Size of data to be encrypted.
	 *
	 * @returns A tuple containing encrypted data.
	 */
	virtual std::tuple<std::shared_ptr<void>, unsigned int> sign(const unsigned char* message,
								     unsigned int messageLength);

	/**
	 * @brief Encrypts data with the key.
	 *
	 * @param data Pointer to the buffer containing data.
	 * @param dataLen Size of data to be encrypted.
	 *
	 * @returns A tuple containing encrypted data.
	 */
	virtual std::tuple<std::shared_ptr<void>, unsigned int>
	  encrypt(const unsigned char* message, unsigned int messageLength) const;

	/**
	 * @brief Decrypts data with the key.
	 *
	 * @param data Pointer to the buffer containing data.
	 * @param dataLen Size of data to be decrypted.
	 *
	 * @returns A tuple containing decrypted data.
	 */
	virtual std::tuple<std::shared_ptr<void>, unsigned int>
	  decrypt(const unsigned char* message, unsigned int messageLength) const;

	/**
	 * @brief Saves the key into a PEM file.
	 *
	 * @param path Path to the filename.
	 * @param cipher String determining which ciphter to use.
	 * @param pwd Password to use to protect the file. If null provided, no password will be used.
	 */
	virtual void savePem(const char* path, const char* cipher, const char* pwd) const;
	
      protected:

	/**
	 * @brief Loads private key data from PEM file.
	 *
	 * @param path The path to the file.
	 * @param pwd Password to open the file.
	 */
	void loadPem(const char* path, const char* pwd);

	/**
	 * @brief Initializes internal RSA structure from the provided one (std::move is used to ensure private key data
	 *   is kept in one place only.
	 *
	 * @param rsa Source RSA.
	 * @param bits Length of the key.
	 */
	void init(RsaStructUniquePtr &&rsa, int bits);

	/**
	 * @brief Initializes public key data from the private key data.
	 *
	 * This method copies public elements of private key RSA structure into the public key RSA structure.
	 */
	void initPublicKey();

      protected:

	/** @name Public key object */
	std::shared_ptr<RsaPublicKey> myPublicKey;

	//	std::unique_ptr<BIO, decltype(&::BIO_free)> myPublicKey;
	//	std::unique_ptr<BIO, decltype(&::BIO_free)> myPrivateKey;
	
      };
    } // End of OpenSSL
#endif 
  } // End of Crypto
} // End of Engine



#endif // _ENGINE_CRYPTO_OPENSSL_RSAPRIVATEKEY_h_
