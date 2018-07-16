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

#include "cpp/RadJavCPPNetUriParser.h"

#include <regex>
#include <boost/algorithm/string.hpp>

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			static uri_data parse_uri(const String& url)
			{
				uri_data result;

				auto validate_match = [](const std::string& value, std::string&& default_value) -> String
				{
					return (value.empty() ? default_value : value);
				};

				// Note: only "http", "https", "ws", and "wss" protocols are supported
				static const std::regex PARSE_URL{ R"((([httpsw]{2,5})://)?([^/ :]+)(:(\d+))?(/([^ ?]+)?)?/?\??([^/ ]+\=[^/ ]+)?)",
					std::regex_constants::ECMAScript | std::regex_constants::icase };
				std::smatch match;

				if (std::regex_match(url, match, PARSE_URL) && match.size() == 9)
				{
					result.protocol = validate_match(boost::algorithm::to_lower_copy(std::string(match[2])), "http");
					result.host = match[3];
					const bool secure_protocol = (result.protocol == "https" || result.protocol == "wss");
					result.port = validate_match(match[5], (secure_protocol) ? "443" : "80");
					result.resource = validate_match(match[6], "/");
					result.query = match[8];
				}

				return result;
			}
		}
	}
}
	