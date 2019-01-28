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

#ifndef _ENGINE_CRYPTO_IPUBLICKEY_h_
#define _ENGINE_CRYPTO_IPUBLICKEY_h_

#include "IKey.h"

namespace Engine
{
  namespace Crypto
  {
    /**
     * @class IPublicKey
     *
     * @brief An interface to a public key.
     */
    class IPublicKey : virtual public IKey
    {
    public:
      /**
       * @brief Ensure destructor is virtual;
       */
      virtual ~IPublicKey() = default;

      //virtual std::tuple<std::shared_ptr<const void *> keyData, unsigned int keyLength> getDer() = 0;
      //virtual std::shared_ptr<const char> getPem() = 0;

      /**
       * @brief Verify if provided signature was created by the privatekey.
       *
       * @returns true if the signature was created by the private key, false otherwise.
       */
      virtual bool verify(const unsigned char* data,
			  unsigned int dataLength,
			  const unsigned char* signature,
			  unsigned int signatureLength) const = 0;

	/**
	 * @brief Saves the key into a PEM file.
	 *
	 * @param path Path to the filename.
	 */
      virtual void savePem(const char* path) const = 0;
      
    };
  }
}

#endif // _ENGINE_CRYPTO_PUBLICKEY_h_
