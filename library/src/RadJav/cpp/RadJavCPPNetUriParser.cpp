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

#include <algorithm>
#include <regex>
#include <boost/algorithm/string.hpp>

#include <iostream>

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			uri_data parse_uri(const String& url)
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

		        UriParser::UriParser()
			{
			}
		  
		        UriParser::UriParser(const std::string &uri)
			{
			  parse(uri);
			}

		        void UriParser::parse(const std::string &uri)
			{

			  unsigned counter = 0;

			  std::regex uriRegex (
					       R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)",
					       std::regex::extended
					       );
			  std::smatch uriMatchResult;

			  if (std::regex_match(uri, uriMatchResult, uriRegex)) {

			    /*
			    for (const auto& res : uriMatchResult) {
			      std::cout << counter++ << ": " << res << std::endl;
			    }
			    */

			    scheme = uriMatchResult[2];
			    authority = uriMatchResult[4];
			    path = uriMatchResult[5];
			    query = uriMatchResult[7];
			    fragment = uriMatchResult[9];


			    // Convert scheme/protocol to lower case
			    std::transform(scheme.begin(), scheme.end(), scheme.begin(), ::tolower);


			    /*
			    std::cout <<std::endl <<std::endl << "First Pass Results: " <<std::endl;
			    std::cout << "Scheme:\t" << scheme <<std::endl;
			    std::cout << "Authority:\t" << authority <<std::endl;
			    std::cout << "Path:\t" << path <<std::endl;
			    std::cout << "Query:\t" << query <<std::endl;
			    std::cout << "Fragment:\t" << fragment <<std::endl;
			    */

			    {
			      std::regex authorityRegex (
							 R"(^(([^\/?#]+)?@)?(([^:]+)(:(.+))?))",
							 std::regex::extended
							 );
			      std::smatch matchResult;

			      if (std::regex_match(authority, matchResult, authorityRegex))
				{
				  //counter = 0;
				  //for (const auto& res : matchResult) 
				  //std::cout << counter++ << ": " << res << std::endl;

				  std::string authData = matchResult[2];
				  host = matchResult[4];
				  port = matchResult[6];

				  if (authData != "")
				    {
				      std::regex authDataRegex (
								R"(^([^ \/?#:]+)(:([^\/?#]+)))",
								std::regex::extended
								);
				      std::smatch matchResult;

				      if (std::regex_match(authData, matchResult, authDataRegex))
					{
					  //std::cout << "Auth Data: " <<std::endl;
					  //counter = 0;
					  //for (const auto& res : matchResult) 
					  //std::cout << counter++ << ": " << res << std::endl;
	      
					  username = matchResult[1];
					  password = matchResult[3];
	    
					}
				      else
					throw std::invalid_argument("Malformed authentication data: " + authority + ", Expecting user[:password]  format");

				    } // End of if (authData != "")


				  { // Verify host
				    std::regex expr (
						     R"(^([-a-zA-Z0-9.]+))",
						     std::regex::extended
						     );
				    std::smatch matchResult;


				    if (std::regex_match(host, matchResult, expr))
				      {
				      }
				    else
				      throw std::invalid_argument("Malformed hostname: " + host + ", allowed characters are: ., -, a-z, A-Z, 0-9");

				  }

				  // Verify port	  
				  if (port != "")
				    { 
				      std::regex expr (
						       R"(^([0-9]+))",
						       std::regex::extended
						       );
				      std::smatch matchResult;


				      if (std::regex_match(port, matchResult, expr))
					{

					}
				      else
					throw std::invalid_argument("Malformed port: " + port + ", allowed characters are: 0-9");
				    }
				}
			      else
				throw std::invalid_argument("Malformed authority: " + authority + ", Expecting user:password@host:port format");
			    }
			  }
			  else
			    {
			      throw std::invalid_argument("Malformed uri: " + uri);
			    }



			}
		  
		}
	}
}
	
