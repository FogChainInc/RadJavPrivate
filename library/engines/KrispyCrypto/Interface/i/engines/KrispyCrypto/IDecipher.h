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

#ifndef _ENGINE_CRYPTO_IDECIPHER_h_
#define _ENGINE_CRYPTO_IDECIPHER_h_

#include <memory>
#include <tuple>
#include <string>

namespace Engine
{
  namespace Crypto
  {
    class IKey;

    /**
     * @class IDeipherMultipart
     *
     * @brief An interface to a decipher object which can decrypt data provided to it in multiple chunks.
     *
     * This is useful for dealing with large data or streaming.
     */
    class IDecipherMultipart {
    public:

      /**
       * @brief Ensure destructor is virtual;
       */
      virtual ~IDecipherMultipart() = default;
      
      
      /**
       * @brief Prepares object for another decryption operation.
       */
      virtual void reset() = 0;

      /**
       * @brief Prepares object for another decryption operation.
       */
      virtual std::tuple<std::shared_ptr<void>, unsigned int>
	update(const void* data, int dataLen) = 0;

      /**
       * @brief Decrypts a string.
       */
      virtual std::tuple<std::shared_ptr<void>, unsigned int>
	update(const std::string& str) = 0;
      
      /**
       * @brief Finishes the decryption operation.
       *
       * @returns A tuple containing decrypted data and its size. 
       *
       * The caller owns the resource.
       */
      virtual std::tuple<std::shared_ptr<void>, unsigned int> finalize() = 0;
      
    };

    /**
     * @class IDecipher
     *
     * @brief An interface to a cipher object which can decrypts data in one call.
     *
     * This is useful for dealing with smaller data, that fits in one buffer.
     */
    class IDecipher {
    public:

      /**
       * @brief Ensure destructor is virtual;
       */
      virtual ~IDecipher() = default;

      /**
       * @brief Decrypts a binary chunk of data.
       *
       * @param data Pointer to the buffer containing data.
       * @param dataLen Size of data to be decrypted.
       *
       * @returns A tuple containing decrypted data.
       */
      virtual std::tuple<std::shared_ptr<void>, unsigned int>
	decipher(const void* data, int dataLen) = 0;

      /**
       * @brief Decrypts a string.
       *
       * @param str String to be decrypted.
       *
       * @returns A tuple containing decrypted data.
       */
      virtual std::tuple<std::shared_ptr<void>, unsigned int>
	decipher(const std::string& str) = 0;
    };
  }
}

#endif // _ENGINE_CRYPTO_IDECIPHER_h_
