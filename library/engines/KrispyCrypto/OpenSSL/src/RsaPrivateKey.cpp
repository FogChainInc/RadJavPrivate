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

#include "RsaPrivateKey.h"

#include "../orb/ORB_EngineCryptoOpenSSL.h"

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {

      RsaPrivateKey::RsaPrivateKey(RsaStructUniquePtr rsa, int bits, int encryptPadding, int signatureType) :
      	RsaKey(std::move(rsa), bits, encryptPadding, signatureType)
      //RsaPrivateKey::RsaPrivateKey(RsaStructUniquePtr rsa, int bits) :
	//RsaKey(std::move(rsa), bits),
	//myRsaPublic(RSA_new(), RSA_free)
	//	myPublicKey(NULL, NULL),
	//	myPrivateKey(NULL, NULL)
      {

	const BIGNUM *nFromPriv, *eFromPriv, *dFromPriv;

	RSA_get0_key(myRsa.get(), &nFromPriv, &eFromPriv, &dFromPriv);

	BIGNUM *n = BN_new();
	BIGNUM *e = BN_new();
	BIGNUM *d = BN_new();

	if (n == NULL || e == NULL || d == NULL)
	  {
	    BN_free(n);
	    BN_free(e);
	    BN_free(d);

	    throw(std::invalid_argument(std::string("Can't allocate memory for RSA public key: ")));
	  }

	if (nFromPriv) BN_copy(n, nFromPriv); else { BN_free(n); n = NULL; }
	if (eFromPriv) BN_copy(e, eFromPriv); else { BN_free(e); e = NULL; }
	if (dFromPriv) BN_copy(d, dFromPriv); else { BN_free(d); d = NULL; }
	
	//myRsaPublic = std::move(RsaStructUniquePtr(RSA_new(), RSA_free));
	RsaStructUniquePtr rsaPublic(RSA_new(), RSA_free);
	
	if (!RSA_set0_key(rsaPublic.get(), n, e, d)) {
	    BN_free(n);
	    BN_free(e);
	    BN_free(d);

	    throw(std::invalid_argument(std::string("Can't set RSA public key: ")));
	}

	myPublicKey = ORB::Engine::Crypto::OpenSSL::createRsaPublicKey(std::move(rsaPublic), bits,
								       encryptPadding, signatureType);
      }
      

      RsaPrivateKey::~RsaPrivateKey()
      {

      }

      void RsaPrivateKey::init(RsaStructUniquePtr &&rsa, int bits)
      {
	myRsa = std::move(rsa);
	myBits = bits;
      }

      std::shared_ptr<const IPublicKey> RsaPrivateKey::getPublicKey() {

	return myPublicKey;
      }
      
      std::tuple<std::shared_ptr<void>, unsigned int> RsaPrivateKey::sign(const unsigned char* message,
									  unsigned int messageLength)
      {
	auto signature =
	  std::shared_ptr<unsigned char>(new unsigned char[RSA_size(myRsa.get())],
					 std::default_delete<unsigned char[]>());

	unsigned int signatureLength;
	if (RSA_sign(mySignatureType, message, messageLength,
		     signature.get(), &signatureLength,
		     myRsa.get()) != 1)
	  throw(std::invalid_argument(std::string("Failed to create the signature. ")));

	return std::make_tuple(signature, signatureLength);

      }
      
      std::tuple<std::shared_ptr<void>, unsigned int>
      RsaPrivateKey::encrypt(const unsigned char* message, unsigned int messageLength) const
      {

	auto cipherText =
	  std::shared_ptr<unsigned char>(new unsigned char[RSA_size(myRsa.get())],
					 std::default_delete<unsigned char[]>());

	int cipherTextLength  = RSA_private_encrypt(messageLength, message, cipherText.get(),
						    myRsa.get(), myEncryptPadding);

	if (cipherTextLength == -1)
	  throw(std::invalid_argument(std::string("Failed to encrypt the message. ")));
	  

	return std::make_tuple(cipherText, cipherTextLength);
	
      }

      std::tuple<std::shared_ptr<void>, unsigned int>
      RsaPrivateKey::decrypt(const unsigned char* message, unsigned int messageLength) const
      {
	auto plainText =
	  std::shared_ptr<unsigned char>(new unsigned char[RSA_size(myRsa.get())],
					 std::default_delete<unsigned char[]>());

	int plainTextLength  = RSA_private_decrypt(messageLength, message, plainText.get(),
						   myRsa.get(), myEncryptPadding);

	if (plainTextLength == -1)
	  throw(std::invalid_argument(std::string("Failed to decrypt the message. ")));
	
	return std::make_tuple(plainText, plainTextLength);
      }


    } // End of OpenSSL
    #endif 
  } // End of Crypto
} // End of Engine



