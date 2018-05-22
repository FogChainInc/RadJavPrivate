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

#include "Digest.h"

#include <iostream>

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {

      Digest::Digest(const std::string& digestType)
      {
	myDigest = EVP_get_digestbyname(digestType.c_str());

	// WORKAROUND
	// OpenSSL 1.1.1-pre includes sha3 and shake hashes, but  the getdigestbyname()  is broken.
	if (!myDigest)
	  {
	    if (digestType == "sha3_224") myDigest = EVP_sha3_224();
	    else if (digestType == "sha3_256") myDigest = EVP_sha3_256();
	    else if (digestType == "sha3_384") myDigest = EVP_sha3_384();
	    else if (digestType == "sha3_512") myDigest = EVP_sha3_512();
	    
	    else if (digestType == "shake128") myDigest = EVP_shake128();
	    else if (digestType == "shake256") myDigest = EVP_shake256();
	    
	    else throw std::invalid_argument(std::string("No such digest: ") + digestType);
	  }

	myDigestCtx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(myDigestCtx, myDigest, NULL);
	
      }

      void Digest::reset()
      {
	EVP_MD_CTX_reset(myDigestCtx);
	EVP_DigestInit_ex(myDigestCtx, myDigest, NULL);
      }

      void Digest::update(const void* data, int dataLen)
      {
	EVP_DigestUpdate(myDigestCtx, data, dataLen);
      }

      void Digest::update(const std::string& str)
      {
	//std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
	update(str.c_str(), str.length());
      }

      void Digest::update(std::shared_ptr<IKey> key)
      {
	// ToDo
      }

      std::tuple<std::shared_ptr<void>, unsigned int> Digest::finalize()
      {
	//std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
	unsigned int digestLen;

	digestLen = EVP_MD_size(myDigest);

	std::shared_ptr<void> digestValue = std::shared_ptr<unsigned char>(new unsigned char[digestLen],
									   std::default_delete<unsigned char[]>());
	EVP_DigestFinal(myDigestCtx, static_cast<unsigned char*>(digestValue.get()), &digestLen);

	return std::make_tuple(digestValue, digestLen);
      }
      
      
      std::tuple<std::shared_ptr<void>, unsigned int>
      Digest::digest(const void* data, int dataLen)
      {
	//std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
	update(data, dataLen);
	auto ret = finalize();
	reset();

	return ret;

      }

      std::tuple<std::shared_ptr<void>, unsigned int>
      Digest::digest(const std::string& str)
      {
	//std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
	update(str);
	auto ret = finalize();
	reset();

	return ret;
      }

      std::tuple<std::shared_ptr<void>, unsigned int>
      Digest::digest(std::shared_ptr<IKey> key)
      {
	update(key);
	auto ret = finalize();
	reset();

	return ret;
      }	

      Digest::~Digest()
      {
	EVP_MD_CTX_free(myDigestCtx);
      }



    } // End of OpenSSL
    #endif
  } // End of Crypto
} // End of Engine
