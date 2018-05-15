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

#include <i/Engine/Crypto/ICipher.h>
#include <i/Engine/Crypto/IDecipher.h>
#include "../orb/ORB_EngineCryptoOpenSSL.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>

using namespace std;

using namespace ORB::Engine::Crypto::OpenSSL;
using namespace Engine::Crypto;

int main() {
  #ifdef USE_OPENSSL

  {
    shared_ptr<ICipher> c = createCipher("aes-128-cbc", "Beer", "");
    shared_ptr<IDecipher> d = createDecipher("aes-128-cbc", "Beer", "");

    string text = "-1-Cats and Dogs1234567890-2-Cats and Dogs1234567890-3-Cats and Dogs1234567890-4-Cats and Dogs1234567890-5-Cats and Dogs1234567890";

    string textBlock = "Cats and Dogs1234567890";
    string plainText = "";
    for (int i=0; i<5; i++) {
      stringstream text;
      text << "-" << i << "-" << textBlock;
      plainText += text.str();
    }
    cout << "Text to cipher: " << plainText << endl;
    cout << "Plain text length: " << plainText.length() << endl;
    
    auto cipherResult = c -> cipher(plainText);

    shared_ptr<void> cipherText = std::get<0>(cipherResult);
    unsigned int cipherLength = std::get<1>(cipherResult);
    
    cout << "Full cipherText length: " << cipherLength << endl;

    auto decipherResult = d -> decipher(cipherText.get(), cipherLength);

    shared_ptr<void> decipheredText = std::get<0>(decipherResult);
    unsigned int decipheredTextLength = std::get<1>(decipherResult);

    stringstream out;
    for (int i=0; i<decipheredTextLength; i++)
      out << (static_cast<char*>(decipheredText.get()))[i];
    /*
    for (int i=0; i<textLength; i++)
      out << hex << setw(2) << setfill('0') << (int)(static_cast<unsigned char*>(plainText.get()))[i];
    */

    cout << "Deciphered plain text: " << out.str() << endl;
  }
  {
    shared_ptr<ICipherMultipart> c = createCipherMultipart("aes-128-cbc", "Beer", "");
    shared_ptr<IDecipher> d = createDecipher("aes-128-cbc", "Beer", "");

    static const int s_bufSize = 1600;
    unsigned char fullCipherText[s_bufSize];
    int cPos = 0; // Position in the buffer.

    cout << "Text to cipher: ";
    string textBlock = "Cats and Dogs1234567890";
    int fullPlainTextLength = 0;
    for (int i=0; i<5; i++) {
      stringstream text;
      text << "-" << i << "-" << textBlock;
      cout << text.str();
      fullPlainTextLength += text.str().length();

      auto cipherResult = c -> update(text.str());
      std::shared_ptr<void> cipherText = std::get<0>(cipherResult);
      unsigned int cipherLength = std::get<1>(cipherResult);
      std::memcpy(&fullCipherText[cPos], cipherText.get(), cipherLength);
      cPos += cipherLength;
    }
    cout << endl;
    cout << "Plain text length: " << fullPlainTextLength << endl;
    
    auto cipherResult = c -> finalize();
    std::shared_ptr<void> cipherText = std::get<0>(cipherResult);
    unsigned int cipherLength = std::get<1>(cipherResult);
    std::memcpy(&fullCipherText[cPos], cipherText.get(), cipherLength);
    cPos += cipherLength;

    cout << "Full cipherText length: " << cPos << endl;

    auto decipherResult = d -> decipher(fullCipherText, cPos);

    shared_ptr<void> plainText = std::get<0>(decipherResult);
    unsigned int textLength = std::get<1>(decipherResult);

    stringstream out;
    for (int i=0; i<textLength; i++)
      out << (static_cast<char*>(plainText.get()))[i];
    /*
    for (int i=0; i<textLength; i++)
      out << hex << setw(2) << setfill('0') << (int)(static_cast<unsigned char*>(plainText.get()))[i];
    */

    cout << "Deciphered plain text: " << out.str() << endl;
  }

  {
    shared_ptr<ICipher> c = createCipher("aes-128-cbc", "Beer", "");
    shared_ptr<IDecipher> d = createDecipher("aes-128-cbc", "Beer", "");

    string plainText = "asdfadfadsf";

    auto cipherResult = c -> cipher(plainText);

    shared_ptr<void> cipherText = std::get<0>(cipherResult);
    unsigned int cipherLength = std::get<1>(cipherResult);
    
    cout << "Full cipherText length: " << cipherLength << endl;

    auto decipherResult = d -> decipher(cipherText.get(), cipherLength);

    shared_ptr<void> decipheredText = std::get<0>(decipherResult);
    unsigned int decipheredTextLength = std::get<1>(decipherResult);

    stringstream out;
    for (int i=0; i<decipheredTextLength; i++)
      out << (static_cast<char*>(decipheredText.get()))[i];
    /*
    for (int i=0; i<textLength; i++)
      out << hex << setw(2) << setfill('0') << (int)(static_cast<unsigned char*>(plainText.get()))[i];
    */

    cout << "Deciphered plain text: " << out.str() << endl;
  }

	 
  #endif
  return 0;
}
