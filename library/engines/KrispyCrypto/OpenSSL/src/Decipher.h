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

#ifndef _ENGINE_CRYPTO_OPENSSL_DECIPHER_h_
#define _ENGINE_CRYPTO_OPENSSL_DECIPHER_h_

#include <i/engines/KrispyCrypto/IDecipher.h>
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
       * @class Decipher
       *
       * @brief Provides functionality for decrypting data. It implements the multipart and simple (one shot) decryption interfaces.
       */
      class Decipher : virtual public IDecipher,
	virtual public IDecipherMultipart
	{
	public:
	  static const int s_bufSize = 1600;
	  
	  /**
	   * @brief Constructs a Decipher object.
	   *
	   * @param cipherType A string representing the type.
	   * @param key A string representing secret/password.
	   * @param iv A string representing the initialization vector.
	   */
	  Decipher(const std::string& decipherType, const std::string &key, const std::string &iv="");

	  /**
	   * @brief Destroys the object.
	   */
	  virtual ~Decipher();
      
	  /**
	   * @brief Prepares object for another decryption operation.
	   */
	  virtual void reset();
	  
	  /**
	   * @brief Decrypts a binary chunk of data.
	   *
	   * @param data Pointer to the buffer containing data.
	   * @param dataLen Size of data to be decrypted.
	   *
	   * @returns A tuple containing decrypted data.
	   */
	  virtual std::tuple<std::shared_ptr<void>, unsigned int>
	    update(const void* data, int dataLen);

	  /**
	   * @brief Decrypts a chunk of data
	   *
	   * @param str String to be decrypted.
	   *
	   * @returns A tuple containing decrypted data.
	   */
	  virtual std::tuple<std::shared_ptr<void>, unsigned int>
	    update(const std::string& str);
      
	  /**
	   * @brief Finishes the decryption operation.
	   *
	   * @returns A tuple containing decrypted data and its size. 
	   *
	   * The caller owns the resource.
	   */
	  virtual std::tuple<std::shared_ptr<void>, unsigned int> finalize();
	
	  /**
	   * @brief Finishes the decryption operation.
	   *
	   * @returns A tuple containing decrypted data and its size. 
	   *
	   * The caller owns the resource.
	   */
	  virtual std::tuple<std::shared_ptr<void>, unsigned int>
	    decipher(const void* data, int dataLen);

	  /**
	   * @brief Finishes the decryption operation.
	   *
	   * @returns A tuple containing decrypted data and its size. 
	   *
	   * The caller owns the resource.
	   */
	  virtual std::tuple<std::shared_ptr<void>, unsigned int>
	    decipher(const std::string& str);

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
	  unsigned char myPlainText[s_bufSize];
	  unsigned char myPos;
	  //@}
		
      };
    } // End of OpenSSL
    #endif 
  } // End of Crypto
} // End of Engine



#endif // _ENGINE_CRYPTO_OPENSSL_DECIPHER_h_
