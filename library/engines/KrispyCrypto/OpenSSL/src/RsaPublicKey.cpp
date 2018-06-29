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

#include <openssl/pem.h>

namespace Engine
{
  namespace Crypto
  {
    #ifdef USE_OPENSSL
    namespace OpenSSL
    {

      RsaPublicKey::RsaPublicKey(RsaStructUniquePtr rsa, int bits, int encryptPadding, int signatureType) :
      	RsaKey(std::move(rsa), bits, encryptPadding, signatureType)
      {

      }

      RsaPublicKey::RsaPublicKey(const char *path, const char *format, int encryptPadding, int signatureType) :
      	RsaKey(encryptPadding, signatureType)
      {
	if (std::string(format) == "pem")
	  {
	    loadPem(path);
	  }
      }

      RsaPublicKey::~RsaPublicKey()
      {

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

      void RsaPublicKey::savePem(const char* path) const 
      {

	if (path == nullptr)
	  throw(std::invalid_argument(std::string("NULL pointer passed to RsaPublicKey::savePem()")));
	if (path == "")
	  throw(std::invalid_argument(std::string("Empty string passed to RsaPublicKey::savePem()")));

	BioFileUniquePtr pem(BIO_new_file(path, "w+"), ::BIO_free);

	if (pem == nullptr)
	  throw(std::invalid_argument(std::string("Error while trying to write private key to") +
				      path));


	int ret = PEM_write_bio_RSAPublicKey(pem.get(), myRsa.get());
	if (ret != 1) 
	  throw std::invalid_argument(std::string("RsaPublicKey::savePem(): PEM write failed."));

      }
	
      void RsaPublicKey::loadPem(const char* path)
      {
	if (path == nullptr)
	  throw(std::invalid_argument(std::string("NULL pointer passed to RsaPublicKey::loadPem()")));
	if (path == "")
	  throw(std::invalid_argument(std::string("Empty string passed to RsaPublicKey::loadPem()")));

	BioFileUniquePtr pem(BIO_new_file(path, "r"), ::BIO_free);
	
	if (pem == nullptr)
	  throw(std::invalid_argument(std::string("Error while preparing to read public key from: ") +
				      path));

	myRsa = std::move(RsaStructUniquePtr(PEM_read_bio_RSAPublicKey(pem.get(), NULL, NULL, NULL), RSA_free));
	
	if (myRsa == nullptr)
	  throw(std::invalid_argument(std::string("Error while trying to read public key from: ") +
				      path));

	EVP_PKEY_set1_RSA(myEvpPkey.get(), myRsa.get());
      }

    } // End of OpenSSL
    #endif 
  } // End of Crypto
} // End of Engine



