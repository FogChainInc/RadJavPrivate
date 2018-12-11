/*
MIT-LICENSE
Copyright (c) 2017-2018 Higher Edge Software, LLC

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
#ifndef _RADJAV_CPP_NET_URI_PARSER_H_

#include "RadJavString.h"
#include <string>

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			struct uri_data {
				std::string protocol;
				std::string host;
				std::string port;
				std::string resource;
				std::string query;
			};

			uri_data parse_uri(const String& url);



			struct UriParser {
			public:
			  UriParser();
			  UriParser(const std::string &uri);
			  void parse(const std::string &uri);

			  // General URI components
			  std::string scheme; // protocol
			  std::string authority; // user:pwd@host:port
			  std::string path; // also referred to as a target
			  std::string query; // ?a=1&b=2
			  std::string fragment; // #Fragment (from http://ala.com/ola.html#Fragment

			  // URL components
			  std::string username;
			  std::string password;
			  std::string host;
			  std::string port;

			};
		}
	}
}

#endif
