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

#include <iostream>
namespace ORB
{
  namespace Engine
  {
    namespace Crypto
    {

      unsigned char hex2bin(char hexChar)
      {
	if (hexChar >= '0' && hexChar <= '9')
	  return hexChar - '0';
	else if (hexChar >= 'A' && hexChar <= 'F')
	  return hexChar - 'A' + 10;
	else if (hexChar >= 'a' && hexChar <= 'f') 
	  return hexChar - 'a' + 10;

	// Non-hex character
	throw std::invalid_argument(std::string("Non-hex character: ") + hexChar);
      }

      std::string encodeHex(const void* data, int dataLen)
      {
	std::string out(dataLen*2, ' ');

	if (!data || dataLen == 0)
	  return ""; // TODO - throw an exception here?

	for (int i=0; i<dataLen; i++) {
	  out[i*2]   = "0123456789ABCDEF"[(static_cast<const unsigned char*>(data))[i] >> 4];
	  out[i*2+1] = "0123456789ABCDEF"[(static_cast<const unsigned char*>(data))[i] & 0x0F];

	}
	out[dataLen * 2] = '\0';

	return out;
      }
      
      std::tuple<std::shared_ptr<void>, unsigned int> decodeHex(const std::string& hexString)
      {

	int dataLen = hexString.length()/2;
	
	std::shared_ptr<unsigned char> dataHex = std::shared_ptr<unsigned char>(new unsigned char[dataLen],
										std::default_delete<unsigned char[]>());

	for (int i=0; i<dataLen; i++)
	  {
	    // TODO - may want to catch exception here and rethrow with more info.
	    unsigned char byte0 = hex2bin(hexString[i*2]);
	    unsigned char byte1 = hex2bin(hexString[i*2+1]);

	    dataHex.get()[i] = (byte0 << 4) | byte1;
	  }
	
	return std::make_tuple(dataHex, dataLen);
      }

      std::tuple<std::shared_ptr<void>, unsigned int> decodeHex(const void *data, int dataLength)
      {

	int dataLen = dataLength/2;
	
	std::shared_ptr<unsigned char> dataHex = std::shared_ptr<unsigned char>(new unsigned char[dataLen],
										std::default_delete<unsigned char[]>());

	auto hexString = static_cast<const char*>(data);
	
	for (int i=0; i<dataLen; i++)
	  {
	    // TODO - may want to catch exception here and rethrow with more info.
	    unsigned char byte0 = hex2bin(hexString[i*2]);
	    unsigned char byte1 = hex2bin(hexString[i*2+1]);

	    dataHex.get()[i] = (byte0 << 4) | byte1;
	  }
	
	return std::make_tuple(dataHex, dataLen);
      }
      

    } // End of Crypto
  } // End of Engine
} // End of ORB
