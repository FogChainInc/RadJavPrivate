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
#include <i/Engine/Crypto/IDigest.h>
#include <i/Engine/Crypto/ICipher.h>
#include <i/Engine/Crypto/IDecipher.h>

namespace ORB
{
  namespace Engine
  {
    namespace Crypto
    {
      std::shared_ptr<::Engine::Crypto::IDigest>
	createDigest(const std::string &digestType,
		     const std::string &cryptoLibrary = "OpenSSL");

      std::shared_ptr<::Engine::Crypto::IDigestMultipart>
	createDigestMultipart(const std::string &digestType,
			      const std::string &cryptoLibrary = "OpenSSL");
      
      std::map<std::string, std::string> getListOfDigests(const std::string &cryptoLibrary = "OpenSSL");


	std::shared_ptr<::Engine::Crypto::ICipher> createCipher(const std::string &cipherType,
								const std::string &secret,
								const std::string &iv="",
								const std::string &cryptoLibrary = "OpenSSL");
	
	std::shared_ptr<::Engine::Crypto::ICipherMultipart> createCipherMultipart(const std::string& cipherType,
										  const std::string &secret,
										  const std::string &iv="",
										  const std::string &cryptoLibrary = "OpenSSL");
	
	std::shared_ptr<::Engine::Crypto::IDecipher> createDecipher(const std::string& cipherType,
								    const std::string &secret,
								    const std::string &iv="",
								    const std::string &cryptoLibrary = "OpenSSL");
	
	std::shared_ptr<::Engine::Crypto::IDecipherMultipart> createDecipherMultipart(const std::string& cipherType,
										      const std::string &secret,
										      const std::string &iv="",
										      const std::string &cryptoLibrary = "OpenSSL");

	std::map<std::string, std::string> getListOfCiphers(const std::string &cryptoLibrary = "OpenSSL");
      

      std::string encodeHex(const void* data, int dataLen);
      std::tuple<std::shared_ptr<void>, unsigned int> decodeHex(const std::string &hexString);
      std::tuple<std::shared_ptr<void>, unsigned int> decodeHex(const void *data, int dataLength);

      std::string encodeBase64(const void* data, int dataLen);
      std::tuple<std::shared_ptr<void>, unsigned int> decodeBase64(const std::string &base64String);
      std::tuple<std::shared_ptr<void>, unsigned int> decodeBase64(const void *data, int dataLength);

    } // End of Crypto
  } // End of Engine
} // End of ORB

#endif // _ORB_ENGINE_CRYPTO_h_
