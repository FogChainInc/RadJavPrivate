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

#include <i/Engine/Crypto/IDigest.h>
#include "../orb/ORB_EngineCryptoOpenSSL.h"


#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

string getHex(shared_ptr<void> digestResult, unsigned int digestLengh);

int main()
{
  #ifdef USE_OPENSSL
  
  string text = "Beer is good!";

  std::map<std::string, std::string> digests = ORB::Engine::Crypto::OpenSSL::getListOfDigests();

  
  for (const auto& digest : digests) {
    cout << digest.first << ":\t" << digest.second << endl;

    try {
      shared_ptr<Engine::Crypto::IDigest> d = ORB::Engine::Crypto::OpenSSL::createDigest(digest.first);

      auto result = d -> digest(text);

      shared_ptr<void> digestResult = get<0>(result);
      unsigned int digestLength = get<1>(result);

      cout << getHex(digestResult, digestLength);
    
      cout << endl << endl;
    } catch (exception& e) {

      cout << "No such digest: " << digest.first << endl;
    }
      
	return 0;
  }

  #endif 
  return 0;
}

string getHex(shared_ptr<void> digestResult, unsigned int digestLength)
{
  stringstream out;
  for (int i=0; i<digestLength; i++)
    out << hex << setw(2) << setfill('0') << (int)(static_cast<unsigned char*>(digestResult.get()))[i];

  return out.str();
}
