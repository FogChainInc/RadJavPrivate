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

#ifndef _ENGINE_CRYPTO_ICERTIFICATE_h_
#define _ENGINE_CRYPTO_ICERTIFICATE_h_

#include <memory>
#include <tuple>

namespace Engine
{
  namespace Crypto
  {

    /**
     * @class ICertificate
     * 
     * @brief An interface to a Public Key Infrastructure Certificate object, such us X509 certificate.
     */				  
    class ICertificate
    {
    public:
      virtual ~ICertificate() = default;

      /**
       * @brief Verifies whether the provided public key is certified by this object
       *
       * @param key Public Key to be verified.
       *
       * @returns true if certificate matches the key, false otherwise.
       */
      virtual bool verify(const IPublicKey *key) = 0;

      /**
       * @brief Returns certificate data in DER format.
       *
       * @returns A tuple containing certificate data and its size.
       */
      virtual std::tuple<const void *certData, unsigned int certLength> getDer() = 0;

      /**
       * @brief Returns certificate data in PEM format.
       *
       * @returns A reference to string containing PEM data.
       */
      virtual const char *getPem() = 0;

      /**
       * @brief return an interface to the public key associated with this certificate.
       *
       * @returns IPublicKey object.
       */
      virtual const IPublicKey *getPublicKey() = 0;
      
    };

  }
}

#endif // _ENGINE_CRYPTO_CERTIFICATE_h_
