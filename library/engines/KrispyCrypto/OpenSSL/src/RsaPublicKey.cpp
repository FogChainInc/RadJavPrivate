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

#include "RsaPublicKey.h"

#include "../orb/ORB_EngineCryptoOpenSSL.h"

#include <iostream>

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {

      RsaPublicKey::RsaPublicKey(RsaStructUniquePtr rsa, int bits, int encryptPadding, int signatureType) :
      	RsaKey(std::move(rsa), bits, encryptPadding, signatureType)
      //RsaPublicKey::RsaPublicKey(RsaStructUniquePtr rsa, int bits) :
	//RsaKey(std::move(rsa), bits),
	//myRsaPublic(RSA_new(), RSA_free)
	//	myPublicKey(NULL, NULL),
	//	myPublicKey(NULL, NULL)
      {

      }
      

      RsaPublicKey::~RsaPublicKey()
      {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
      }

      bool RsaPublicKey::verify(const unsigned char* message,
		  unsigned int messageLength,
		  const unsigned char* signature,
		  unsigned int signatureLength) const
      {
	return RSA_verify(mySignatureType, message, messageLength,
			  signature, signatureLength,
			  myRsa.get());
      }

      std::tuple<std::shared_ptr<void>, unsigned int>
      RsaPublicKey::encrypt(const unsigned char* message, unsigned int messageLength) const
      {

	auto cipherText =
	  std::shared_ptr<unsigned char>(new unsigned char[RSA_size(myRsa.get())],
					 std::default_delete<unsigned char[]>());

	int cipherTextLength  = RSA_public_encrypt(messageLength, message, cipherText.get(),
						   myRsa.get(), myEncryptPadding);

	if (cipherTextLength == -1)
	  throw(std::invalid_argument(std::string("Failed to encrypt the message. ")));
	  

	return std::make_tuple(cipherText, cipherTextLength);
	
      }

      std::tuple<std::shared_ptr<void>, unsigned int>
      RsaPublicKey::decrypt(const unsigned char* message, unsigned int messageLength) const
      {
	auto plainText =
	  std::shared_ptr<unsigned char>(new unsigned char[RSA_size(myRsa.get())],
					 std::default_delete<unsigned char[]>());

	int plainTextLength  = RSA_public_decrypt(messageLength, message, plainText.get(),
						  myRsa.get(), myEncryptPadding);

	if (plainTextLength == -1)
	  throw(std::invalid_argument(std::string("Failed to decrypt the message. ")));
	
	return std::make_tuple(plainText, plainTextLength);
      }

      

    } // End of OpenSSL
    #endif 
  } // End of Crypto
} // End of Engine



