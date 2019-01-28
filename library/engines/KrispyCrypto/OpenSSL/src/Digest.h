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

#ifndef _ENGINE_CRYPTO_OPENSSL_DIGEST_h_
#define _ENGINE_CRYPTO_OPENSSL_DIGEST_h_

#include <i/engines/KrispyCrypto/IDigest.h>
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
       * @class Digest
       *
       * @brief Provides functionality for hash data. It implements the multipart and simple (one shot) digest interfaces.
       */
      class Digest : virtual public IDigest,
	virtual public IDigestMultipart
	{
	public:

	  /**
	   * @brief Constructs a Digest object.
	   *
	   * @param digestType A string representing the type.
	   */
	  Digest(const std::string& hashType);

	  /**
	   * @brief Destroys the object.
	   */
	  virtual ~Digest();
      
	  /**
	   * @brief Prepares object for another hash operation.
	   */
	  virtual void reset();

	  /**
	   * @brief Hashes a binary chunk of data.
	   *
	   * @param data Pointer to the buffer containing data.
	   * @param dataLen Size of data to be hashed.
	   */
	  virtual void update(const void* data, int dataLen);

	  /**
	   * @brief Hashes a string.
	   *
	   * @param str String to be hashed.
	   */
	  virtual void update(const std::string& str);

	  /**
	   * @brief Hashes a key.
	   */
	  virtual void update(std::shared_ptr<IKey>);
      
	  /**
	   * @brief Finishes the hash operation.
	   *
	   * @returns A tuple containing hashed data and its size. 
	   *
	   * The caller owns the resource.
	   */
	  virtual std::tuple<std::shared_ptr<void>, unsigned int> finalize();
	
	  /**
	   * @brief Hashes a binary chunk of data.
	   *
	   * @param data Pointer to the buffer containing data.
	   * @param dataLen Size of data to be hashed.
	   *
	   * @returns A tuple containing hashed data.
	   */
	  virtual std::tuple<std::shared_ptr<void>, unsigned int>
	    digest(const void* data, int dataLen);

	  /**
	   * @brief Hashes a string.
	   *
	   * @param str String to be hashed.
	   *
	   * @returns A tuple containing hashed data.
	   */
	  virtual std::tuple<std::shared_ptr<void>, unsigned int>
	    digest(const std::string& str);

	  /**
	   * @brief Hashes a key..
	   *
	   * @param key Key to be hashed.
	   *
	   * @returns A tuple containing hashed data.
	   */
	  virtual std::tuple<std::shared_ptr<void>, unsigned int>
	    digest(std::shared_ptr<IKey> key);

	private:
	  /** @name Hash specification and structures */
	  //@{
	  std::string myHashType;

	  const EVP_MD *myDigest;
	  EVP_MD_CTX *myDigestCtx; // digest context
	  //@}
	
	};
    } // End of OpenSSL
#endif 
  } // End of Crypto
} // End of Engine



#endif // _ENGINE_CRYPTO_OPENSSL_DIGEST_h_
