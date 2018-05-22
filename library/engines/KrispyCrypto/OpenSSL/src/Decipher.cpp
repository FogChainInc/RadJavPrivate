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

#include "Decipher.h"
#include "../orb/ORB_EngineCryptoOpenSSL.h"

#include <cstring>
#include <sstream>
#include <iostream>

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {

      Decipher::Decipher(const std::string &cipherType, const std::string &key, const std::string &iv) :
	myPos(0)
      {
	myCipherType = cipherType;
	myCipher = EVP_get_cipherbyname(cipherType.c_str());

	// WORKAROUND
	// OpenSSL 1.1.1-pre includes sha3 and shake hashes, but  the getcipherbyname()  is broken.
	if (!myCipher)
	  {
	    //if (cipherType == "sha3_224") myCipher = EVP_sha3_224();
	    //else if (cipherType == "sha3_256") myCipher = EVP_sha3_256();
	    throw std::invalid_argument(std::string("No such cipher: ") + cipherType);
	  }

	if (! (myCipherCtx = EVP_CIPHER_CTX_new()) )
	  throw std::invalid_argument(std::string("Couldn't create cipher context for: ") + cipherType);

	int keyLen = EVP_CIPHER_key_length(myCipher);
	int ivLen = EVP_CIPHER_iv_length(myCipher);
	myBlockSize = EVP_CIPHER_block_size(myCipher);

	// Just pick a good hash for the time being.
	auto digest = ORB::Engine::Crypto::OpenSSL::createDigest("sha512");

	myKeyDigest = digest -> digest(key);
	if (std::get<1>(myKeyDigest) < keyLen)
	  throw std::invalid_argument(static_cast<std::stringstream&&>(std::stringstream()
								       << "Couldn't create key for: " << cipherType <<
								       ", required length is: " << keyLen).str());
	
	myKeyData = static_cast<unsigned char*>(std::get<0>(myKeyDigest).get());

	myIvData = NULL;
	if (iv != "")
	  {
	    myIvDigest = digest -> digest(iv);
	    
	    if (std::get<1>(myIvDigest) < ivLen)
	      throw std::invalid_argument(static_cast<std::stringstream&&>(std::stringstream()
								       << "Couldn't create iv for: " << cipherType <<
									   ", required length is: " << ivLen).str());
	    
	    myIvData = static_cast<unsigned char*>(std::get<0>(myIvDigest).get());
	  }
	  
	// Note, at this point we know we have enough bits in myKeyData and myIvData.
	if (EVP_DecryptInit_ex(myCipherCtx, myCipher, NULL, myKeyData, myIvData) != 1)
	  throw std::invalid_argument(std::string("Couldn't initialize cipher context for: ") + cipherType);

      }

      void Decipher::reset()
      {
	EVP_CIPHER_CTX_reset(myCipherCtx);
	if (EVP_DecryptInit_ex(myCipherCtx, myCipher, NULL, myKeyData, myIvData) != 1)
	  throw std::invalid_argument(std::string("Couldn't initialize cipher context for: ") + myCipherType);
      }

      std::tuple<std::shared_ptr<void>, unsigned int>
      Decipher::update(const void* data, int dataLen)
      {
	int textLen;
	std::shared_ptr<unsigned char> decipheredText =
	  std::shared_ptr<unsigned char>(new unsigned char[dataLen + myBlockSize],
					 std::default_delete<unsigned char[]>());

	EVP_DecryptUpdate(myCipherCtx, decipheredText.get(), &textLen,
			  static_cast<const unsigned char*>(data), dataLen);

	return std::make_tuple(decipheredText, textLen);
      }

      std::tuple<std::shared_ptr<void>, unsigned int>
      Decipher::update(const std::string& str)
      {
	//std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
	return update(str.c_str(), str.length());

      }

      std::tuple<std::shared_ptr<void>, unsigned int> Decipher::finalize()
      {
	//std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
	int textLen;
	std::shared_ptr<unsigned char> decipheredText =
	  std::shared_ptr<unsigned char>(new unsigned char[myBlockSize],
					 std::default_delete<unsigned char[]>());

	EVP_DecryptFinal_ex(myCipherCtx, decipheredText.get(), &textLen);

	return std::make_tuple(decipheredText, textLen);
      }
      
      
      std::tuple<std::shared_ptr<void>, unsigned int>
      Decipher::decipher(const void* data, int dataLen)
      {
	auto decipherResult = update(data, dataLen);
	std::shared_ptr<void> decipheredText = std::get<0>(decipherResult);
	unsigned int decipheredTextLength = std::get<1>(decipherResult);
	
	auto decipherPadding = finalize();
	std::shared_ptr<void> paddingText = std::get<0>(decipherPadding);
	unsigned int paddingLength = std::get<1>(decipherPadding);

	int fullDecipheredTextLength = decipheredTextLength + paddingLength;

	std::shared_ptr<unsigned char> fullDecipheredText =
	  std::shared_ptr<unsigned char>(new unsigned char[fullDecipheredTextLength],
					 std::default_delete<unsigned char[]>());

	std::memcpy(fullDecipheredText.get(), decipheredText.get(), decipheredTextLength);
	std::memcpy(&fullDecipheredText.get()[decipheredTextLength], paddingText.get(), paddingLength);

	std::cout << "Full decipheredText length: " << fullDecipheredTextLength
		  << ", and the text:" << std::endl;
	std::stringstream out;
	for (int i=0; i<fullDecipheredTextLength; i++)
	  out << fullDecipheredText.get()[i];
	std::cout  << out.str() << std::endl;
	
	
	reset();

	return std::make_tuple(fullDecipheredText, fullDecipheredTextLength);

      }

      std::tuple<std::shared_ptr<void>, unsigned int>
      Decipher::decipher(const std::string& str)
      {
	//std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
	update(str);
	auto ret = finalize();
	reset();

	return ret;
      }

      Decipher::~Decipher()
      {
	EVP_CIPHER_CTX_cleanup(myCipherCtx);
	EVP_CIPHER_CTX_free(myCipherCtx);
      }



    } // End of OpenSSL
    #endif
  } // End of Crypto
} // End of Engine
