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

#ifndef _ENGINE_CRYPTO_OPENSSL_CIPHER_h_
#define _ENGINE_CRYPTO_OPENSSL_CIPHER_h_

#include <i/Engine/Crypto/ICipher.h>
#include <openssl/evp.h>

#include <string>

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {
      class Cipher : virtual public ICipher,
	virtual public ICipherMultipart
	{
	public:
	  static const int s_bufSize = 1600;
	  
	  Cipher(const std::string& cipherType, const std::string &key, const std::string &iv="");

	  virtual ~Cipher();
      
	  virtual void reset();
	  virtual std::tuple<std::shared_ptr<void>, unsigned int>
	    update(const void* data, int dataLen);
	  virtual std::tuple<std::shared_ptr<void>, unsigned int>
	    update(const std::string& str);
      
	  virtual std::tuple<std::shared_ptr<void>, unsigned int> finalize();
	
	  virtual std::tuple<std::shared_ptr<void>, unsigned int>
	    cipher(const void* data, int dataLen);
	  virtual std::tuple<std::shared_ptr<void>, unsigned int>
	    cipher(const std::string& str);

	private:
	  std::string myCipherType;

	  const EVP_CIPHER *myCipher;
	  EVP_CIPHER_CTX *myCipherCtx; // cipher context

	  std::tuple<std::shared_ptr<void>, unsigned int> myKeyDigest;
	  std::tuple<std::shared_ptr<void>, unsigned int> myIvDigest;
	  unsigned char* myKeyData;
	  unsigned char* myIvData;

	  int myBlockSize;
	  unsigned char myCipherText[s_bufSize];
	  unsigned char myPos;
		
      };
    } // End of OpenSSL
    #endif 
  } // End of Crypto
} // End of Engine



#endif // _ENGINE_CRYPTO_OPENSSL_CIPHER_h_
