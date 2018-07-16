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

#include "../orb/ORB_EngineCryptoOpenSSL.h"
#include "../src/RsaKeyGenerator.h"
#include "../src/RsaPrivateKey.h"
#include "../src/RsaPublicKey.h"
#include "../src/Digest.h"

#include <openssl/obj_mac.h>

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

int main() {
  #ifdef USE_OPENSSL

  //auto keyGenerator = ORB::Engine::Crypto::OpenSSL::createRsaKeyGenerator("rsa-f4", RSA_PKCS1_PADDING, NID_md5_sha1);
  //auto keyGenerator = ORB::Engine::Crypto::OpenSSL::createRsaKeyGenerator(RSA_F4, RSA_PKCS1_PADDING, 1);
  auto keyGenerator = ORB::Engine::Crypto::OpenSSL::createRsaKeyGenerator(2048);

  {
    auto privateKey = keyGenerator -> generate();

    privateKey -> savePem("PrivateKey.pem", nullptr, nullptr);

    unsigned char msg[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

    auto encrypted = privateKey -> encrypt(msg, 10);

    cout << "Encrypted length" << std::get<1>(encrypted) << endl;
    
    const unsigned char *encryptedMsg = static_cast<const unsigned char*>(std::get<0>(encrypted).get());
				   
    unsigned int encryptedMsgLength = std::get<1>(encrypted);

    auto publicKey = privateKey -> getPublicKey();
    auto decrypted = publicKey -> decrypt(encryptedMsg, encryptedMsgLength);

    publicKey -> savePem("PublicKey.pem");
							     
    const unsigned char *ptr = static_cast<const unsigned char*>(std::get<0>(decrypted).get());

    unsigned int decryptedLength = std::get<1>(decrypted);

    cout << "Decrypted length: " << decryptedLength << endl;

    for (int i=0; i<10; i++)
      cout << (int)ptr[i] << " ";
    cout << endl;

    try
      {

	Engine::Crypto::OpenSSL::Digest d("md5");
	auto result = d.digest(msg, 10);

	unsigned char *digestResult = static_cast<unsigned char*>(std::get<0>(result).get());
	unsigned int digestLength = std::get<1>(result);
	cout << "Digest length: " << digestLength << endl;

	auto signature = privateKey -> sign(digestResult, digestLength);
	cout << "Signature length: " << std::get<1>(signature) << endl;
    
	const unsigned char *signatureData = static_cast<const unsigned char*>(std::get<0>(signature).get());
				   
	unsigned int signatureLength = std::get<1>(signature);

	if (publicKey -> verify(digestResult, digestLength, signatureData, signatureLength))
	  cout << "Signature verified" << endl;
	else
	  cout << "Signature verification failure" << endl;
      }
    catch (std::exception& e)
      {
	cout << "Exception: " << e.what() << endl;
      }

    try
      {
	auto privateKey = ORB::Engine::Crypto::OpenSSL::createRsaPrivateKey("PrivateKey.pem", "pem", nullptr);

	privateKey -> savePem("PrivateKey2.pem", nullptr, nullptr);

	auto publicKey = ORB::Engine::Crypto::OpenSSL::createRsaPublicKey("PublicKey.pem", "pem");


	Engine::Crypto::OpenSSL::Digest d("md5");
	auto result = d.digest(msg, 10);

	unsigned char *digestResult = static_cast<unsigned char*>(std::get<0>(result).get());
	unsigned int digestLength = std::get<1>(result);
	cout << "Digest length: " << digestLength << endl;

	auto signature = privateKey -> sign(digestResult, digestLength);
	cout << "Signature length: " << std::get<1>(signature) << endl;
    
	const unsigned char *signatureData = static_cast<const unsigned char*>(std::get<0>(signature).get());
				   
	unsigned int signatureLength = std::get<1>(signature);

	if (publicKey -> verify(digestResult, digestLength, signatureData, signatureLength))
	  cout << "Signature verified" << endl;
	else
	  cout << "Signature verification failure" << endl;
	
	

      }
    catch (std::exception& e)
      {
	cout << "Exception: " << e.what() << endl;
      }
    

  }
	 
  #endif
  return 0;
}
