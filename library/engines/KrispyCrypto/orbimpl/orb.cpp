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

#include <orb/ORB_EngineCrypto.h>

#include <i/engines/KrispyCrypto/IDigest.h>
#include <i/engines/KrispyCrypto/ICipher.h>
#include <i/engines/KrispyCrypto/IDecipher.h>
#include <i/engines/KrispyCrypto/IKeyGenerator.h>
#include <i/engines/KrispyCrypto/IPrivateKey.h>
#include <i/engines/KrispyCrypto/IPublicKey.h>

#include "../OpenSSL/orb/ORB_EngineCryptoOpenSSL.h"

#include "../OpenSSL/src/RsaKeyGenerator.h"
#include "../OpenSSL/src/RsaPrivateKey.h"
#include "../OpenSSL/src/RsaPublicKey.h"

namespace ORB
{
  namespace Engine
  {
    namespace Crypto
    {

      std::shared_ptr<::Engine::Crypto::IDigest> createDigest(const std::string& digestType,
							      const std::string& cryptoImplementation)
      {

        #ifdef USE_OPENSSL
	return OpenSSL::createDigest(digestType);
	#else
	return nullptr;
        #endif

      }

      std::shared_ptr<::Engine::Crypto::IDigestMultipart> createDigestMultipart(const std::string& digestType,
										const std::string& cryptoImplementation)
      {

	#ifdef USE_OPENSSL
	return OpenSSL::createDigestMultipart(digestType);
	#else
	return nullptr;
        #endif

      }

      std::map<std::string, std::string> getListOfDigests(const std::string& cryptoImplementation)
      {

	#ifdef USE_OPENSSL
	return OpenSSL::getListOfDigests();
	#else
	return std::map<std::string, std::string>();
	#endif
	
      }

      std::shared_ptr<::Engine::Crypto::ICipher> createCipher(const std::string &cipherType,
							      const std::string &secret,
							      const std::string &iv,
							      const std::string &cryptoLibrary)
      {

	#ifdef USE_OPENSSL
	return OpenSSL::createCipher(cipherType, secret, iv);
	#else
	return nullptr;
        #endif

      }

      std::shared_ptr<::Engine::Crypto::ICipherMultipart> createCipherMultipart(const std::string &cipherType,
										const std::string &secret,
										const std::string &iv,
										const std::string &cryptoLibrary)
      {

	#ifdef USE_OPENSSL
	return OpenSSL::createCipherMultipart(cipherType, secret, iv);
	#else
	return nullptr;
        #endif

      }
      
      std::shared_ptr<::Engine::Crypto::IDecipher> createDecipher(const std::string &cipherType,
								  const std::string &secret,
								  const std::string &iv,
								  const std::string &cryptoLibrary)
      {

	#ifdef USE_OPENSSL
	return OpenSSL::createDecipher(cipherType, secret, iv);
	#else
	return nullptr;
        #endif

      }

      std::shared_ptr<::Engine::Crypto::IDecipherMultipart> createDecipherMultipart(const std::string &cipherType,
										    const std::string &secret,
										    const std::string &iv,
										    const std::string &cryptoLibrary)
      {

	#ifdef USE_OPENSSL
	return OpenSSL::createDecipherMultipart(cipherType, secret, iv);
	#else
	return nullptr;
        #endif

      }
      

      std::map<std::string, std::string> getListOfCiphers(const std::string& cryptoImplementation)
      {

	#ifdef USE_OPENSSL
	return OpenSSL::getListOfCiphers();
	#else
	return std::map<std::string, std::string>();
	#endif
	
      }

      std::shared_ptr<::Engine::Crypto::IKeyGenerator> createRsaKeyGenerator(int bits,
									     std::string pubExponentStr,
									     std::string encryptPaddingStr,
									     std::string signatureTypeStr)
      {
	#ifdef USE_OPENSSL

	unsigned long pubExponent;

	if (pubExponentStr ==  "") pubExponentStr = "RSA_F4";
	if (pubExponentStr == "RSA_3")
	  pubExponent = RSA_3;
	else if (pubExponentStr == "RSA_F4")
	  pubExponent = RSA_F4;
	else throw std::invalid_argument(std::string("Unsupported public exponent: ") + std::to_string(pubExponent));

	int encryptPadding;

	if (encryptPaddingStr == "") encryptPaddingStr = "RSA_PKCS1_PADDING";
	if (encryptPaddingStr == "RSA_PKCS1_PADDING")
	  encryptPadding = RSA_PKCS1_PADDING;
	else if (encryptPaddingStr == "RSA_NO_PADDING")
	  encryptPadding = RSA_NO_PADDING;
	else 
	  throw std::invalid_argument(std::string("Invalid encrypt padding: ") + encryptPaddingStr);

	int signatureType;

	if (signatureTypeStr == "") signatureTypeStr = "OPENSSL_DEFAULT";
	if (signatureTypeStr == "OPENSSL_DEFAULT")
	  signatureType = NID_sha256; // Just any valid value different from SSL signature
	else if (signatureTypeStr == "NID_md5_sha1")
	  signatureType = NID_md5_sha1;
	else
	  throw std::invalid_argument(std::string("Invalid signature type: ") + signatureTypeStr);
	    
	return OpenSSL::createRsaKeyGenerator(bits, pubExponent, encryptPadding, signatureType);
	
	#else
	return nullptr;
	#endif

      }

      std::shared_ptr<::Engine::Crypto::IKeyGenerator> createKeyGenerator(std::map<std::string, std::string> parms,
									  std::string cryptoLibrary)
      {
	#ifdef USE_OPENSSL

	std::string algorithm;
	std::map<std::string, std::string>::iterator i;

	i =  parms.find("algorithm");
	if (i == parms.end())
	  throw std::invalid_argument(std::string("The parameter 'algorithm' must be defined"));
	algorithm = i -> second;

	
	if (algorithm != "RSA" && algorithm != "ECC")
	  throw std::invalid_argument(std::string("Invalid algorithm: ") + algorithm +
				      std::string(". Supported values: RSA, ECC"));


	if (algorithm == "RSA")
	  {
	    i =  parms.find("bits");
	    if (i == parms.end())
	      throw std::invalid_argument(std::string("The parameter 'bits' must be defined"));

	    int bits;
	    try
	      {
		bits = std::stoi(i -> second);
	      }
	    catch (std::exception &e)
	      {
		throw std::invalid_argument(std::string("Bad value for 'bits' parameter: ") + i -> second);
	      }

	    std::string pubExponent;
	    std::string encryptPadding;
	    std::string signatureType;

	    if ((i =  parms.find("pubExponent")) != parms.end())
	      pubExponent = i -> second;
	    if ((i =  parms.find("encryptPadding")) != parms.end())
	      encryptPadding = i -> second;
	    if ((i =  parms.find("signatureType")) != parms.end())
	      signatureType = i -> second;


	    return createRsaKeyGenerator(bits, pubExponent, encryptPadding, signatureType);

	  }
	
	#else
	return nullptr;
	#endif
	
      }

      std::shared_ptr<::Engine::Crypto::IPrivateKey> createPrivateKey(std::map<std::string, std::string> parms,
								      std::string cryptoLibrary)
      {
	#ifdef USE_OPENSSL

	std::string algorithm;
	std::string path;
	std::string format;
	
	std::map<std::string, std::string>::iterator i;
	
	i =  parms.find("path");
	if (i == parms.end())
	  throw std::invalid_argument(std::string("The parameter 'path' must be defined"));
	path = i -> second;

	i =  parms.find("format");
	if (i == parms.end())
	  throw std::invalid_argument(std::string("The parameter 'format' must be defined"));
	format = i -> second;
	
	i =  parms.find("algorithm");
	if (i == parms.end())
	  throw std::invalid_argument(std::string("The parameter 'algorithm' must be defined"));
	algorithm = i -> second;
	
	if (algorithm != "RSA" && algorithm != "ECC")
	  throw std::invalid_argument(std::string("Invalid algorithm: ") + algorithm +
				      std::string(". Supported values: RSA, ECC"));

	const char *pwd = NULL;

	if ((i =  parms.find("password")) != parms.end())
	  pwd = i -> second.c_str();

	if (algorithm == "RSA")
	  {
	
	    std::string encryptPaddingStr = OPENSSL_DEFAULT_ENCRYPT_PADDING;
	    std::string signatureTypeStr = OPENSSL_DEFAULT_SIGNATURE_TYPE;

	    if ((i =  parms.find("encryptPadding")) != parms.end())
	      encryptPaddingStr = i -> second;
	    if ((i =  parms.find("signatureType")) != parms.end())
	      signatureTypeStr = i -> second;

	    int signatureType;

	    if (signatureTypeStr == "OPENSSL_DEFAULT")
	      signatureType = NID_sha256; // Just any valid value different from SSL signature
	    else if (signatureTypeStr == "NID_md5_sha1")
	      signatureType = NID_md5_sha1;
	    else
	      throw std::invalid_argument(std::string("Invalid signature type: ") + signatureTypeStr);
	    
	    int encryptPadding;
	    if (encryptPaddingStr == "RSA_PKCS1_PADDING")
	      encryptPadding = RSA_PKCS1_PADDING;
	    else if (encryptPaddingStr == "RSA_NO_PADDING")
	      encryptPadding = RSA_NO_PADDING;
	    else 
	      throw std::invalid_argument(std::string("Invalid encrypt padding: ") + encryptPaddingStr);
	    
	    return OpenSSL::createRsaPrivateKey(path.c_str(), format.c_str(), pwd, 
						encryptPadding, signatureType);
	  }

	#else
	return nullptr;
	#endif
	
      }

      std::shared_ptr<::Engine::Crypto::IPublicKey> createPublicKey(std::map<std::string, std::string> parms,
								    std::string cryptoLibrary)
      {
	#ifdef USE_OPENSSL

	std::string algorithm;
	std::string path;
	std::string format;
	
	std::map<std::string, std::string>::iterator i;
	
	i =  parms.find("path");
	if (i == parms.end())
	  throw std::invalid_argument(std::string("The parameter 'path' must be defined"));
	path = i -> second;

	i =  parms.find("format");
	if (i == parms.end())
	  throw std::invalid_argument(std::string("The parameter 'format' must be defined"));
	format = i -> second;
	
	i =  parms.find("algorithm");
	if (i == parms.end())
	  throw std::invalid_argument(std::string("The parameter 'algorithm' must be defined"));
	algorithm = i -> second;
	
	if (algorithm != "RSA" && algorithm != "ECC")
	  throw std::invalid_argument(std::string("Invalid algorithm: ") + algorithm +
				      std::string(". Supported values: RSA, ECC"));


	if (algorithm == "RSA")
	  {
	
	    std::string encryptPaddingStr = OPENSSL_DEFAULT_ENCRYPT_PADDING;
	    std::string signatureTypeStr = OPENSSL_DEFAULT_SIGNATURE_TYPE;

	    if ((i =  parms.find("encryptPadding")) != parms.end())
	      encryptPaddingStr = i -> second;
	    if ((i =  parms.find("signatureType")) != parms.end())
	      signatureTypeStr = i -> second;

	    int signatureType;

	    if (signatureTypeStr == "OPENSSL_DEFAULT")
	      signatureType = NID_sha256; // Just any valid value different from SSL signature
	    else if (signatureTypeStr == "NID_md5_sha1")
	      signatureType = NID_md5_sha1;
	    else
	      throw std::invalid_argument(std::string("Invalid signature type: ") + signatureTypeStr);
	    
	    int encryptPadding;
	    if (encryptPaddingStr == "RSA_PKCS1_PADDING")
	      encryptPadding = RSA_PKCS1_PADDING;
	    else if (encryptPaddingStr == "RSA_NO_PADDING")
	      encryptPadding = RSA_NO_PADDING;
	    else 
	      throw std::invalid_argument(std::string("Invalid encrypt padding: ") + encryptPaddingStr);
	    
	    return OpenSSL::createRsaPublicKey(path.c_str(), format.c_str(),
					       encryptPadding, signatureType);
	  }

	#else
	return nullptr;
	#endif
      }      
    } // End of Crypto
  } // End of Engine
} // End of ORB
