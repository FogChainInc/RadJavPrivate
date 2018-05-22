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

#include <orb/ORB_EngineCrypto.h>

#include "../OpenSSL/orb/ORB_EngineCryptoOpenSSL.h"

namespace ORB
{
  namespace Engine
  {
    namespace Crypto
    {

      std::shared_ptr<::Engine::Crypto::IDigest> createDigest(const std::string& digestType,
							      const std::string& cryptoImplementation)
      {

        #ifdef USE_OPENSSL
	return OpenSSL::createDigest(digestType);
	#else
	return nullptr;
        #endif

      }

      std::shared_ptr<::Engine::Crypto::IDigestMultipart> createDigestMultipart(const std::string& digestType,
										const std::string& cryptoImplementation)
      {

	#ifdef USE_OPENSSL
	return OpenSSL::createDigestMultipart(digestType);
	#else
	return nullptr;
        #endif

      }

      std::map<std::string, std::string> getListOfDigests(const std::string& cryptoImplementation)
      {

	#ifdef USE_OPENSSL
	return OpenSSL::getListOfDigests();
	#else
	return std::map<std::string, std::string>();
	#endif
	
      }

      std::shared_ptr<::Engine::Crypto::ICipher> createCipher(const std::string &cipherType,
							      const std::string &secret,
							      const std::string &iv,
							      const std::string &cryptoLibrary)
      {

	#ifdef USE_OPENSSL
	return OpenSSL::createCipher(cipherType, secret, iv);
	#else
	return nullptr;
        #endif

      }

      std::shared_ptr<::Engine::Crypto::ICipherMultipart> createCipherMultipart(const std::string &cipherType,
										const std::string &secret,
										const std::string &iv,
										const std::string &cryptoLibrary)
      {

	#ifdef USE_OPENSSL
	return OpenSSL::createCipherMultipart(cipherType, secret, iv);
	#else
	return nullptr;
        #endif

      }
      
      std::shared_ptr<::Engine::Crypto::IDecipher> createDecipher(const std::string &cipherType,
								  const std::string &secret,
								  const std::string &iv,
								  const std::string &cryptoLibrary)
      {

	#ifdef USE_OPENSSL
	return OpenSSL::createDecipher(cipherType, secret, iv);
	#else
	return nullptr;
        #endif

      }

      std::shared_ptr<::Engine::Crypto::IDecipherMultipart> createDecipherMultipart(const std::string &cipherType,
										    const std::string &secret,
										    const std::string &iv,
										    const std::string &cryptoLibrary)
      {

	#ifdef USE_OPENSSL
	return OpenSSL::createDecipherMultipart(cipherType, secret, iv);
	#else
	return nullptr;
        #endif

      }
      

      std::map<std::string, std::string> getListOfCiphers(const std::string& cryptoImplementation)
      {

	#ifdef USE_OPENSSL
	return OpenSSL::getListOfCiphers();
	#else
	return std::map<std::string, std::string>();
	#endif
	
      }
      
    } // End of Crypto
  } // End of Engine
} // End of ORB
