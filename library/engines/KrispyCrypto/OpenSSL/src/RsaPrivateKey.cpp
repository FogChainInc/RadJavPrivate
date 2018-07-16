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

#include <openssl/pem.h>

#include <cstring>
#include <iostream>

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {

      RsaPrivateKey::RsaPrivateKey(RsaStructUniquePtr rsa, int bits, int encryptPadding, int signatureType) :
      	RsaKey(std::move(rsa), bits, encryptPadding, signatureType)
      {
	initPublicKey();

	std::cout << "RSA SIZE: " << myBits << std::endl;	
      }
      
      RsaPrivateKey::RsaPrivateKey(const char *path, const char *format, const char *pwd, int encryptPadding, int signatureType) :
      	RsaKey(encryptPadding, signatureType) 
      {
	if (path == nullptr)
	  throw(std::invalid_argument(std::string("NULL path pointer passed to RsaPrivateKey(path...)")));
	if (path == "")
	  throw(std::invalid_argument(std::string("Empty path string passed to RsaPrivateKey(path...)")));

	if (format == nullptr)
	  throw(std::invalid_argument(std::string("NULL format pointer passed to RsaPrivateKey(path...)")));
	if (format == "")
	  throw(std::invalid_argument(std::string("Empty format string passed to RsaPrivateKey(path...)")));
	
	if (std::string(format) == "pem")
	  {
	    loadPem(path, pwd);
	  }
	
	initPublicKey();
	myBits = RSA_size(myRsa.get()) * sizeof(char);

	std::cout << "RSA SIZE: " << myBits << std::endl;	
      }

      RsaPrivateKey::~RsaPrivateKey()
      {

      }

      void RsaPrivateKey::initPublicKey()
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

	myPublicKey = ORB::Engine::Crypto::OpenSSL::createRsaPublicKey(std::move(rsaPublic), myBits,
								       myEncryptPadding, mySignatureType);

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


      void RsaPrivateKey::savePem(const char* path, const char* cipherType, const char* pwd) const 
      {

	const EVP_CIPHER *cipher = NULL;

	std::unique_ptr<char, decltype(std::default_delete<char[]>())> localPwd = nullptr;
	int localPwdLength = 0;
	if (cipherType)
	  {
	    EVP_get_cipherbyname(cipherType);

	    if (!cipher)
	      throw std::invalid_argument(std::string("RsaPrivateKey::savePem(): No such cipher: ") + cipherType);

	    if (!pwd)
	      throw std::invalid_argument(std::string("RsaPrivateKey::savePem(): Password must be provided for the cipeher: ") + cipherType);

	    localPwd = std::move(std::unique_ptr<char, decltype(std::default_delete<char[]>())>(new char[std::strlen(pwd) + 1],
						       std::default_delete<char[]>()));
	    localPwdLength = std::strlen(localPwd.get());
	    
	  }
	
	if (path == nullptr)
	  throw(std::invalid_argument(std::string("NULL pointer passed to RsaPrivateKey::savePem()")));
	if (path == "")
	  throw(std::invalid_argument(std::string("Empty string passed to RsaPrivateKey::savePem()")));

	BioFileUniquePtr pem(BIO_new_file(path, "w+"), ::BIO_free);
	if (pem == nullptr)
	  throw(std::invalid_argument(std::string("Error while trying to write private key to") +
				      path));

	int ret = PEM_write_bio_PKCS8PrivateKey(pem.get(), myEvpPkey.get(), cipher,
						localPwd.get(), localPwdLength,
						NULL, NULL);

	if (ret != 1) 
	  throw std::invalid_argument(std::string("RsaPrivateKey::savePem(): PEM write failed."));

      }
	
      void RsaPrivateKey::loadPem(const char *path, const char *pwd)
      {

	if (path == nullptr)
	  throw(std::invalid_argument(std::string("NULL pointer passed to RsaPrivateKey::loadPem()")));
	if (path == "")
	  throw(std::invalid_argument(std::string("Empty string passed to RsaPrivateKey::loadPem()")));

	std::unique_ptr<char, decltype(std::default_delete<char[]>())> localPwd = nullptr;
	int localPwdLength = 0;

	if (pwd)
	  {
	    localPwdLength = std::strlen(pwd);

	    // Ignore if password is an empty string.
	    if (localPwdLength > 0)
	      localPwd =
		std::move(std::unique_ptr<char, decltype(std::default_delete<char[]>())>(new char[localPwdLength + 1],
											 std::default_delete<char[]>()));
	  }

	std::cout << "PRIVATEEEEEEEEEEE KEEEEY PWD: " << std::hex << (void*)localPwd.get() << std::dec << std::endl;
	
	BioFileUniquePtr pem(BIO_new_file(path, "r"), ::BIO_free);

	if (pem == nullptr)
	  throw(std::invalid_argument(std::string("Error while preparing to write private key to") +
				      path));

	myRsa = std::move(RsaStructUniquePtr(PEM_read_bio_RSAPrivateKey(pem.get(), NULL, NULL, localPwd.get()), RSA_free));

	if (myRsa == nullptr) 
	  throw std::invalid_argument(std::string("Error while trying to read private key from: ") + path);

	EVP_PKEY_set1_RSA(myEvpPkey.get(), myRsa.get());

      }

    } // End of OpenSSL
    #endif 
  } // End of Crypto
} // End of Engine



