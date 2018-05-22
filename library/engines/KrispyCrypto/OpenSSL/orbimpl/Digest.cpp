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

#include "../src/Digest.h"
#include "../orb/ORB_EngineCryptoOpenSSL.h"

namespace ORB
{
  namespace Engine
  {
    namespace Crypto
    {
      
      #ifdef USE_OPENSSL
      
      namespace OpenSSL
      {
	std::shared_ptr<::Engine::Crypto::IDigest> createDigest(const std::string& digestType)
	{
	  return std::make_shared<::Engine::Crypto::OpenSSL::Digest>(digestType.c_str());
	}

	std::shared_ptr<::Engine::Crypto::IDigestMultipart> createDigestMultipart(const std::string& digestType)
	{
	  return std::make_shared<::Engine::Crypto::OpenSSL::Digest>(digestType.c_str());
	}

	// TODO - OpenSSL 1.1.1-pre5 doesn't have an API to return available digests.
	// The mechanisms are listed in apps/progs.h in a static function table, and some
	// of supported ones aren't even included there. Since we have to address that anyways,
	// for the time being this list has to be maintained "by hand."
	std::map<std::string, std::string> getListOfDigests() {
	  return std::map<std::string, std::string>
	    {
	      {"blake2b512", "BLAKE2, 512bit"},
	      {"blake2s256", "BLAKE2, 256bit"},
		//{"gost", "GOST R 34.11-2012, 256bit"},
	      {"md4", "MD4, 128bit"},
	      {"md5", "MD5, 128bit"},
	      {"mdc2", "MDC-2/Meyer-Schilling, 128bit"},
	      {"rmd160","RIPEMD-160, 160bit"},
	      {"sha1", "SHA-1, 160bit"},
	      {"sha224", "SHA-2, 224bit"},
	      {"sha256", "SHA-2, 256bit"},
	      {"sha384", "SHA-2, 384bit"},
	      {"sha512", "SHA-2, 512bit"},
	      {"sm3", "SM3 (Chinese Standard), similar to SHA-2, 256bit"},

	      // Missing from OpenSSL 1.1.1-pre5 list
	      {"sha3_224", "SHA-3, 224bit"},
	      {"sha3_256", "SHA-3, 256bit"},
	      {"sha3_384", "SHA-3, 384bit"},
	      {"sha3_512", "SHA-3, 312bit"},
	      {"shake128", "SHA-3 Family, extendable-output function, 128bit strenth"},
	      {"shake256", "SHA-3 Family, extendable-output function, 256bit strenth"}
	    };

	  }

      } // End of OpenSSL
      
      #endif // USE_OPENSSL
      
      
    } // End of Crypto
  } // End of Engine
} // End of ORB
