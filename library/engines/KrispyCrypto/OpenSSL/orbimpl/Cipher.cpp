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

#include "../src/Cipher.h"
#include "../src/Decipher.h"
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
	std::shared_ptr<::Engine::Crypto::ICipher> createCipher(const std::string &cipherType,
								const std::string &secret,
								const std::string &iv)
	{
	  return std::make_shared<::Engine::Crypto::OpenSSL::Cipher>(cipherType, secret, iv);
	}
	
	std::shared_ptr<::Engine::Crypto::ICipherMultipart> createCipherMultipart(const std::string& cipherType,
										  const std::string &secret,
										  const std::string &iv)
	{
	  return std::make_shared<::Engine::Crypto::OpenSSL::Cipher>(cipherType, secret, iv);
	}
	
	std::shared_ptr<::Engine::Crypto::IDecipher> createDecipher(const std::string& cipherType,
								    const std::string &secret,
								    const std::string &iv)
	{
	  return std::make_shared<::Engine::Crypto::OpenSSL::Decipher>(cipherType, secret, iv);
	}
	  
	std::shared_ptr<::Engine::Crypto::IDecipherMultipart> createDecipherMultipart(const std::string& cipherType,
										      const std::string &secret,
										      const std::string &iv)
	{
	  return std::make_shared<::Engine::Crypto::OpenSSL::Decipher>(cipherType, secret, iv);
	}


	// TODO - OpenSSL 1.1.1-pre5 doesn't have an API to return available digests.
	// The mechanisms are listed in apps/progs.h in a static function table, and some
	// of supported ones aren't even included there. Since we have to address that anyways,
	// for the time being this list has to be maintained "by hand."
	std::map<std::string, std::string> getListOfCiphers()
	{
	  return std::map<std::string, std::string>
	    {
	      {"aes-128-cbc", "aes-128-cbc"},
	      {"aes-128-ecb", "aes-128-ecb"},
	      {"aes-192-cbc", "aes-192-cbc"},
	      {"aes-192-ecb", "aes-192-ecb"},
	      {"aes-256-cbc", "aes-256-cbc"},
	      {"aes-256-ecb", "aes-256-ecb"},

	      {"aria-128-cbc", "aria-128-cbc"},
	      {"aria-128-cfb", "aria-128-cfb"},
	      {"aria-128-ctr", "aria-128-ctr"},
	      {"aria-128-ecb", "aria-128-ecb"},
	      {"aria-128-ofb", "aria-128-ofb"},
	      {"aria-128-cfb1", "aria-128-cfb1"},
	      {"aria-128-cfb8", "aria-128-cbf8"},

	      {"aria-192-cbc", "aria-192-cbc"},
	      {"aria-192-cfb", "aria-192-cfb"},
	      {"aria-192-ctr", "aria-192-ctr"},
	      {"aria-192-ecb", "aria-192-ecb"},
	      {"aria-192-ofb", "aria-192-ofb"},
	      {"aria-192-cfb1", "aria-192-cfb1"},
	      {"aria-192-cfb8", "aria-192-cbf8"},

	      {"aria-256-cbc", "aria-256-cbc"},
	      {"aria-256-cfb", "aria-256-cfb"},
	      {"aria-256-ctr", "aria-256-ctr"},
	      {"aria-256-ecb", "aria-256-ecb"},
	      {"aria-256-ofb", "aria-256-ofb"},
	      {"aria-256-cfb1", "aria-256-cfb1"},
	      {"aria-256-cfb8", "aria-256-cbf8"},

	      {"camelia-128-cbc", "camelia-128-cbc"},
	      {"camelia-128-ecb", "camelia-128-ecb"},
	      {"camelia-192-cbc", "camelia-192-cbc"},
	      {"camelia-192-ecb", "camelia-192-ecb"},
	      {"camelia-256-cbc", "camelia-256-cbc"},
	      {"camelia-256-ecb", "camelia-256-ecb"},
		
	      {"base64", "base64"},
	      //{"zlib", "zlib"},
	      {"des", "des"},
	      {"des3", "des3"},
	      {"desx", "desx"},
	      {"idea", "idea"},
	      {"seed", "seed"},
	      {"rc4", "rc4"},
	      {"rc4-40", "rc4-40"},
	      {"rc2", "rc2"},
	      {"bf", "bf"},
	      {"cast", "cast"},
	      {"rc5", "rc5"},

	      {"des-ecb", "des-ecb"},
	      {"des-ede", "des-ede"},
	      {"des-ede3", "des-ede3"},
	      {"des-cbc", "des-cbc"},
	      {"des-ede-cbc", "des-ede-cbc"},
	      {"des-ede3-cbc", "des-ede3-cbc"},
	      {"des-cfb", "des-cfb"},
	      {"des-ede-cfb", "des-ede-cfb"},
	      {"des-ede3-cfb", "des-ede3-cfb"},
	      {"des-ofb", "des-ofb"},
	      {"des-ede-ofb", "des-ede-ofb"},
	      {"des-ede3-ofb", "des-ede3-ofb"},

	      {"idea-cbc", "idea-cbc"},
	      {"idea-ecb", "idea-ecb"},
	      {"idea-cfb", "idea-cfb"},
	      {"idea-ofb", "idea-ofb"},

	      {"seed-cbc", "seed-cbc"},
	      {"seed-ecb", "seed-ecb"},
	      {"seed-cfb", "seed-cfb"},
	      {"seed-ofb", "seed-ofb"},

	      {"rc2-cbc", "rc2-cbc"},
	      {"rc2-ecb", "rc2-ecb"},
	      {"rc2-cfb", "rc2-cfb"},
	      {"rc2-ofb", "rc2-ofb"},

	      {"rc2-64-ofb", "rc2-64-ofb"},
	      {"rc2-40-ofb", "rc2-40-ofb"},
		
	      {"bf-cbc", "bf-cbc"},
	      {"bf-ecb", "bf-ecb"},
	      {"bf-cfb", "bf-cfb"},
	      {"bf-ofb", "bf-ofb"},

	      {"cast5-cbc", "cast5-cbc"},
	      {"cast5-ecb", "cast5-ecb"},
	      {"cast5-cfb", "cast5-cfb"},
	      {"cast5-ofb", "cast5-ofb"},
	      {"cast-cbc", "cast-cbc"},

	      {"rc5-cbc", "rc5-cbc"},
	      {"rc5-ecb", "rc5-ecb"},
	      {"rc5-cfb", "rc5-cfb"},
	      {"rc5-ofb", "rc5-ofb"},

	      {"sm4-cbc", "sm4-cbc"},
	      {"sm4-ecb", "sm4-ecb"},
	      {"sm4-cfb", "sm4-cfb"},
	      {"sm4-ofb", "sm4-ofb"},
	      {"sm4-ctr", "sm4-ctr"}
	    };

	  }

      } // End of OpenSSL
      
      #endif // USE_OPENSSL
      
      
    } // End of Crypto
  } // End of Engine
} // End of ORB
