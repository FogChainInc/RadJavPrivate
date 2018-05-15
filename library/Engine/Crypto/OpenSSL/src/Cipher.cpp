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

#include "Cipher.h"
#include "../orb/ORB_EngineCryptoOpenSSL.h"

#include <cstring>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {

      Cipher::Cipher(const std::string &cipherType, const std::string &key, const std::string &iv) :
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
	if (EVP_EncryptInit_ex(myCipherCtx, myCipher, NULL, myKeyData, myIvData) != 1)
	  throw std::invalid_argument(std::string("Couldn't initialize cipher context for: ") + cipherType);

      }

      void Cipher::reset()
      {
	EVP_CIPHER_CTX_reset(myCipherCtx);
	if (EVP_EncryptInit_ex(myCipherCtx, myCipher, NULL, myKeyData, myIvData) != 1)
	  throw std::invalid_argument(std::string("Couldn't initialize cipher context for: ") + myCipherType);
      }

      std::tuple<std::shared_ptr<void>, unsigned int>
      Cipher::update(const void* data, int dataLen)
      {
	int cipherLen;
	std::shared_ptr<unsigned char> cipherText =
	  std::shared_ptr<unsigned char>(new unsigned char[dataLen + myBlockSize],
					 std::default_delete<unsigned char[]>());
	
	EVP_EncryptUpdate(myCipherCtx, cipherText.get(), &cipherLen,
			  static_cast<const unsigned char*>(data), dataLen);

	std::cout << "Cipher::update: cipherLen: " << cipherLen << std::endl;

	return std::make_tuple(cipherText, cipherLen);
      }

      std::tuple<std::shared_ptr<void>, unsigned int>
      Cipher::update(const std::string& str)
      {
	//std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
	return update(str.c_str(), str.length());
      }

      std::tuple<std::shared_ptr<void>, unsigned int>
      Cipher::finalize()
      {
	//std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
	int cipherLen;
	std::shared_ptr<unsigned char> cipherText =
	  std::shared_ptr<unsigned char>(new unsigned char[myBlockSize],
					 std::default_delete<unsigned char[]>());

	EVP_EncryptFinal_ex(myCipherCtx, cipherText.get(), &cipherLen);

	std::cout << "Cipher::finalize: cipherLen: " << cipherLen << std::endl;
	return std::make_tuple(cipherText, cipherLen);
      }
      
      
      std::tuple<std::shared_ptr<void>, unsigned int>
      Cipher::cipher(const void* data, int dataLen)
      {
	std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
	auto cipherResult = update(data, dataLen);
	std::shared_ptr<void> cipherText = std::get<0>(cipherResult);
	unsigned int cipherLength = std::get<1>(cipherResult);
	
	auto cipherPadding = finalize();
	std::shared_ptr<void> paddingText = std::get<0>(cipherPadding);
	unsigned int paddingLength = std::get<1>(cipherPadding);

	int fullCipherLength = cipherLength + paddingLength;
	std::shared_ptr<unsigned char> fullCipherText =
	  std::shared_ptr<unsigned char>(new unsigned char[fullCipherLength],
					 std::default_delete<unsigned char[]>());

	std::memcpy(fullCipherText.get(), cipherText.get(), cipherLength);
	std::memcpy(&fullCipherText.get()[cipherLength], paddingText.get(), paddingLength);

	std::cout << "Full cipherText length: " << fullCipherLength << ", now hex of ciphertext:" << std::endl;
	std::stringstream out;
	for (int i=0; i<fullCipherLength; i++)
	  out << std::hex << std::setw(2) << std::setfill('0') << (int)(fullCipherText.get())[i];
	std::cout  << out.str() << std::endl;
	
	
	reset();

	return std::make_tuple(fullCipherText, fullCipherLength);

      }

      std::tuple<std::shared_ptr<void>, unsigned int>
      Cipher::cipher(const std::string& str)
      {
	//std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
	return cipher(str.c_str(), str.length());
      }

      Cipher::~Cipher()
      {
	EVP_CIPHER_CTX_cleanup(myCipherCtx);
	EVP_CIPHER_CTX_free(myCipherCtx);
      }



    } // End of OpenSSL
    #endif
  } // End of Crypto
} // End of Engine
