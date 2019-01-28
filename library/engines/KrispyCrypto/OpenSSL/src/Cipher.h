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

#ifndef _ENGINE_CRYPTO_OPENSSL_CIPHER_h_
#define _ENGINE_CRYPTO_OPENSSL_CIPHER_h_

#include <i/engines/KrispyCrypto/ICipher.h>
#include <openssl/evp.h>

#include <string>

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {
      
      /**
       * @class Cipher
       *
       * @brief Provides functionality for encrypting data. It implements the multipart and simple (one shot) encryption interfaces.
       */
      class Cipher : virtual public ICipher,
	virtual public ICipherMultipart
	{
	public:
	  /**
	   * @brief Constructs a Cipher object.
	   *
	   * @param cipherType A string representing the type.
	   * @param key A string representing secret/password.
	   * @param iv A string representing the initialization vector.
	   */
	  Cipher(const std::string& cipherType, const std::string &key, const std::string &iv="");

	  /**
	   * @brief Destroys the object.
	   */
	  virtual ~Cipher();
      
	  /**
	   * @brief Prepares object for another encryption operation.
	   */
	  virtual void reset();

	  /**
	   * @brief Encrypts a binary chunk of data.
	   *
	   * @param data Pointer to the buffer containing data.
	   * @param dataLen Size of data to be encrypted.
	   *
	   * @returns A tuple containing encrypted data.
	   */
	  virtual std::tuple<std::shared_ptr<void>, unsigned int>
	    update(const void* data, int dataLen);

	  
	  /**
	   * @brief Encrypts a chunk of data
	   *
	   * @param str String to be encrypted.
	   *
	   * @returns A tuple containing encrypted data.
	   */
	  virtual std::tuple<std::shared_ptr<void>, unsigned int>
	    update(const std::string& str);
      
	  /**
	   * @brief Finishes the encryption operation.
	   *
	   * @returns A tuple containing encrypted data and its size. 
	   *
	   * The caller owns the resource.
	   */
	  virtual std::tuple<std::shared_ptr<void>, unsigned int> finalize();
	
	  /**
	   * @brief Finishes the encryption operation.
	   *
	   * @returns A tuple containing encrypted data and its size. 
	   *
	   * The caller owns the resource.
	   */
	  virtual std::tuple<std::shared_ptr<void>, unsigned int>
	    cipher(const void* data, int dataLen);

	  /**
	   * @brief Finishes the encryption operation.
	   *
	   * @returns A tuple containing encrypted data and its size. 
	   *
	   * The caller owns the resource.
	   */
	  virtual std::tuple<std::shared_ptr<void>, unsigned int>
	    cipher(const std::string& str);

	private:

	  /** @name Cipher specification and structures */
	  //@{
	  std::string myCipherType;

	  const EVP_CIPHER *myCipher;
	  EVP_CIPHER_CTX *myCipherCtx; // cipher context
	  //@}

	  /** @name Key generation */
	  //@{
	  std::tuple<std::shared_ptr<void>, unsigned int> myKeyDigest;
	  std::tuple<std::shared_ptr<void>, unsigned int> myIvDigest;
	  unsigned char* myKeyData;
	  unsigned char* myIvData;
	  //@}
	  
	  /** @name Buffer */
	  //@{
	  int myBlockSize;
	  //@}
	};
    } // End of OpenSSL
#endif 
  } // End of Crypto
} // End of Engine



#endif // _ENGINE_CRYPTO_OPENSSL_CIPHER_h_
