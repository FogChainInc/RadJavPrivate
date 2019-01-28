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

#ifndef _ENGINE_CRYPTO_IPRIVATEKEY_h_
#define _ENGINE_CRYPTO_IPRIVATEKEY_h_

#include "IKey.h"

namespace Engine
{
  namespace Crypto
  {
    class IPublicKey;
    
    /**
     * @class IPrivateKey
     *
     * @brief An interface to a private key.
     *
     */
    class IPrivateKey : virtual public IKey
    {
    public:
      /**
       * @brief Ensure destructor is virtual;
       */
      virtual ~IPrivateKey() = default;

      /**
       * @brief Provides access to the public key
       *
       * @returns A public key object
       */
      virtual std::shared_ptr<const IPublicKey> getPublicKey() = 0;

      /**
       * @brief Signs data with the key.
       *
       * @param data Pointer to the buffer containing data.
       * @param dataLen Size of data to be encrypted.
       *
       * @returns A tuple containing encrypted data.
       */
      virtual std::tuple<std::shared_ptr<void>, unsigned int> sign(const unsigned char* data,
								   unsigned int dataLength) = 0;

      /**
       * @brief Saves the key into a PEM file.
       *
       * @param path Path to the filename.
       * @param cipher String determining which ciphter to use.
       * @param pwd Password to use to protect the file. If null provided, no password will be used.
       */
       virtual void savePem(const char* path, const char* cipher, const char* pwd) const = 0;
      
   };
  }
}

#endif // _ENGINE_CRYPTO_PRIVATEKEY_h_
