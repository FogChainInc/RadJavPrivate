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

#include "base64.h"


namespace ORB
{
  namespace Engine
  {
    namespace Crypto
    {

      std::string encodeBase64(const void* data, int dataLen)
      {
	size_t outputLength;
	char *base64String = base64_encode(static_cast<const unsigned char*>(data), dataLen,
					   &outputLength);

	return base64String;

      }
      
      std::tuple<std::shared_ptr<void>, unsigned int> decodeBase64(const std::string& hexString)
      {

	size_t outputLength;
	unsigned char *decodedData = base64_decode(hexString.c_str(),
					  hexString.length(),
					  &outputLength);
	
	std::shared_ptr<unsigned char> dataBase64 = std::shared_ptr<unsigned char>(decodedData,
									     free);

	return std::make_tuple(dataBase64, outputLength);
      }

      std::tuple<std::shared_ptr<void>, unsigned int> decodeBase64(const void* data, int dataLength)
      {

	size_t outputLength;
	unsigned char *decodedData = base64_decode(static_cast<const char*>(data),
					  dataLength,
					  &outputLength);
	
	std::shared_ptr<unsigned char> dataBase64 = std::shared_ptr<unsigned char>(decodedData,
									     free);

	return std::make_tuple(dataBase64, outputLength);
      }
      
    } // End of Crypto
  } // End of Engine
} // End of ORB
