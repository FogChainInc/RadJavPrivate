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

#include "../src/Digest.h"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

int main() {
  #ifdef USE_OPENSSL

  Engine::Crypto::OpenSSL::Digest d("sha256");

  {

    auto result = d.digest("Ala ma kota");

    shared_ptr<void> digestResult = std::get<0>(result);
    unsigned int digestLength = std::get<1>(result);

    stringstream out;
    for (int i=0; i<digestLength; i++)
      out << hex << setw(2) << setfill('0') << (int)(static_cast<unsigned char*>(digestResult.get()))[i];

    cout << out.str() << endl;
  }


  {

    auto result = d.digest("Ala ma kotaa");

    shared_ptr<void> digestResult = std::get<0>(result);
    unsigned int digestLength = std::get<1>(result);

    stringstream out;
    for (int i=0; i<digestLength; i++)
      out << hex << setw(2) << setfill('0') << (int)(static_cast<unsigned char*>(digestResult.get()))[i];

    cout << out.str() << endl;
  }

  {
	  Engine::Crypto::OpenSSL::Digest d("sha256");
	  d.update("Ala ma ");
	  d.update("kotaa");

	  auto result = d.finalize();
	  shared_ptr<void> digestResult = std::get<0>(result);
	  unsigned int digestLength = std::get<1>(result);

	  stringstream out;
	  for (int i = 0; i<digestLength; i++)
		  out << hex << setw(2) << setfill('0') << (int)(static_cast<unsigned char*>(digestResult.get()))[i];

	  cout << out.str() << endl;
  }
	 
  #endif
  return 0;
}
